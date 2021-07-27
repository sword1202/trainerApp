//
// Created by Semyon Tikhonenko on 1/15/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "Executors.h"
#include "VocalTrainerFilePlayer.h"
#include "StringUtils.h"
#include "AudioFilePlayer.h"
#include "VocalPartAudioPlayer.h"
#include "MvxFile.h"
#include "Primitives.h"
#include "TimeUtils.h"
#include "Functions.h"
#include "Executors.h"
#include "PlaybackBounds.h"
#include "AudioAverageInputLevelMonitor.h"
#include "VocalTrainerPlayerPrepareException.h"
#include <iostream>
#include <memory>
#include "Logic/PitchesMutableList.h"
#include "AudioOperationFailedException.h"
#include "MathUtils.h"

using namespace CppUtils;
using std::cout;
using std::endl;

VocalTrainerFilePlayer::VocalTrainerFilePlayer() : metronomeEnabled(false) {
}

void VocalTrainerFilePlayer::setSource(VocalTrainerFile *file, bool destroyFileOnDestructor) {
    clearSource();
    setSourceInternal(file, destroyFileOnDestructor);
}

void VocalTrainerFilePlayer::setSourceInternal(VocalTrainerFile *file, bool destroyFileOnDestructor) {
    this->file = file;
    this->destroyFileOnDestructor = destroyFileOnDestructor;
    players.clear();
    AudioDataBufferConstPtr instrumental = file->getInstrumental();
    if (instrumental) {
        instrumentalPlayer.setAudioData(instrumental);
        players.push_back(&instrumentalPlayer);
    } else {
        instrumentalPlayer.setAudioData(nullptr);
    }

    vocalPartPianoPlayer.setVocalPart(file->getVocalPart());
    players.push_back(&vocalPartPianoPlayer);
    delete pitchesCollection;
    pitchesCollection = nullptr;
    if (file->isRecording()) {
        pitchesCollection = new PitchesMutableList(file->getRecordedPitchesFrequencies(),
                                                   file->getRecordedPitchesTimes());
        recordingPlayer.setAudioData(file->getRecordingData());
        players.push_back(&recordingPlayer);

        if (recordingLevelMonitor == nullptr) {
            recordingLevelMonitor = new AudioAverageInputLevelMonitor([=] (double level) {
                recordingVoiceLevelListeners.executeAll(level);
            });
            recordingPlayer.onDataSentToOutputListeners.addListener([=] (void* data, int size) {
                recordingLevelMonitor->operator()(static_cast<const int16_t *>(data), size / sizeof(int16_t));
            });
        }
    }

    if (metronomePlayer.isPrepared()) {
        players.push_back(&metronomePlayer);
    }

    BaseAudioPlayer* mainPlayer = getMainPlayer();
    mainPlayer->seekChangedListeners.clear();
    mainPlayer->onCompleteListeners.clear();
    mainPlayer->onPlaybackStartedListeners.clear();
    mainPlayer->seekChangedListeners.addListener([=](double seek, double) {
        executeOnMainThread([=] {
            if (players.empty()) {
                return;
            }

            if (bounds) {
                if (seek >= bounds.getEndSeek()) {
                    // avoid multiple onComplete calls
                    if (pauseRequestedCounter == 0) {
                        this->onComplete();
                    }
                }
            }

            if (isRecording()) {
                const auto& tonalityChanges = this->file->getRecordingTonalityChanges();
                auto iter = tonalityChanges.upper_bound(seek);
                if (iter != tonalityChanges.begin()) {
                    --iter;
                    setPitchShiftInSemiTones(iter->second);
                } else {
                    setPitchShiftInSemiTones(0);
                }
            } else if(pauseRequestedCounter == 0) {
                // Cut tonality changes
                auto iter = tonalityChanges.upper_bound(seek);
                tonalityChanges.erase(iter, tonalityChanges.end());
            }

            if (lyricsPlayer) {
                lyricsPlayer->setSeek(seek);
            }
        });

        seekChangedListeners.executeAll(seek);
    });

    mainPlayer->onCompleteListeners.addListener([=] {
        this->onComplete();
    });

    mainPlayer->onPlaybackStartedListeners.addListener([=] {
        this->onPlaybackStarted();
    });

    delete lyricsPlayer;
    lyricsPlayer = nullptr;
    const Lyrics& lyrics = file->getLyrics();
    if (!lyrics.isEmpty()) {
        lyricsPlayer = new LyricsPlayer(&lyrics);
        lyricsPlayer->onSelectionChanged = [this] (const LyricsPlayer::Selection& selection) {
            this->lyricsSelectionChangedListeners.executeAll(selection);
        };
        lyricsPlayer->onLinesChanged = [this] (const LyricsDisplayedLinesProvider* linesProvider) {
            this->currentLyricsLinesChangedListeners.executeAll(linesProvider);
        };
    }

    onSourceChanged.executeAll();
}

void VocalTrainerFilePlayer::setSource(std::istream &is) {
    auto* file = VocalTrainerFile::read(is);
    setSource(file);
}

void VocalTrainerFilePlayer::setSource(const char *filePath) {
    std::fstream is = Streams::OpenFile(filePath, std::ios::in | std::ios::binary);
    setSource(is);
}

void VocalTrainerFilePlayer::onComplete() {
    onPlaybackStopped();
    onCompleteListeners.executeAll();
}

void VocalTrainerFilePlayer::pausePlayer(BaseAudioPlayer *player) {
    executeOnBackgroundThread([=] {
        player->pause();
        executeOnMainThread([=] {
            if (--pauseRequestedCounter == 0) {
                onPlaybackStopped();
            }
        });
    });
}

void VocalTrainerFilePlayer::pause() {
    if (!isPlaying() || pauseRequestedCounter != 0) {
        return;
    }

    stopRequestedListeners.executeAll();

    pauseRequestedCounter = players.size();
    for (auto* player : players) {
        pausePlayer(player);
    }
}

void VocalTrainerFilePlayer::play() {
    playRequestedListeners.executeAll();
    updateMetronomeVolume();

    BaseAudioPlayer* mainPlayer = getMainPlayer();
    if (bounds) {
        double seekValue = mainPlayer->getSeek();
        if (!bounds.isInside(seekValue)) {
            double startSeek = bounds.getStartSeek();
            mainPlayer->setSeek(startSeek);
        }
    }

    double seek = mainPlayer->getSeek();
    vocalPartPianoPlayer.setSeek(seek);
    if (metronomePlayer.isPrepared()) {
        metronomePlayer.setSeek(seek);
    }
    if (isRecording()) {
        recordingPlayer.setSeek(seek);
    }

    for (BaseAudioPlayer* player : players) {
        player->play();
    }
}

void VocalTrainerFilePlayer::setSeek(double value) {
    if (value == getMainPlayer()->getSeek()) {
        return;
    }

    value = Math::CutIfOutOfClosedRange(value,
            bounds ? bounds.getStartSeek() : 0,
            bounds ? bounds.getEndSeek() : getMainPlayer()->getOriginalTrackDurationInSeconds());

    for (auto* player : players) {
        player->setSeek(value);
    }
    seekChangedFromUserListeners.executeAll(value);
}

void VocalTrainerFilePlayer::setInstrumentalVolume(float instrumentalVolume) {
    instrumentalPlayer.setVolume(instrumentalVolume);
    updateMetronomeVolume();
}

void VocalTrainerFilePlayer::setVocalPartPianoVolume(float pianoVolume) {
    vocalPartPianoPlayer.setVolume(pianoVolume);
    updateMetronomeVolume();
}

float VocalTrainerFilePlayer::getInstrumentalVolume() const {
    return instrumentalPlayer.getVolume();
}

float VocalTrainerFilePlayer::getVocalPianoVolume() const {
    return vocalPartPianoPlayer.getVolume();
}

void VocalTrainerFilePlayer::setRecordingVolume(float volume) {
    recordingPlayer.setVolume(volume);
    updateMetronomeVolume();
}

VocalTrainerFilePlayer::~VocalTrainerFilePlayer() {
    delete pitchesCollection;
    delete recordingLevelMonitor;
    delete lyricsPlayer;
    if (destroyFileOnDestructor) {
        delete file;
    }
}

void VocalTrainerFilePlayer::prepare() {
    try {
        if (instrumentalPlayer.getAudioData() != nullptr) {
            instrumentalPlayer.prepare();
        }
    } catch (AudioOperationFailedException& e) {
        throw VocalTrainerPlayerPrepareException(VocalTrainerPlayerPrepareException::BROKEN_INSTRUMENTAL, e.what());
    }
    try {
        vocalPartPianoPlayer.prepare();
    } catch (AudioOperationFailedException& e) {
        throw VocalTrainerPlayerPrepareException(VocalTrainerPlayerPrepareException::BROKEN_VOCAL_PART, e.what());
    }
    if (isRecording()) {
        try {
            recordingPlayer.prepare();
        } catch (AudioOperationFailedException& e) {
            throw VocalTrainerPlayerPrepareException(VocalTrainerPlayerPrepareException::BROKEN_RECORDING, e.what());
        }
    }

    if (metronomePlayer.isPrepared()) {
        metronomePlayer.setAudioDataInfo(getBeatsPerMinute(), getMainPlayer()->getTrackDurationInSecondsWithTempoApplied());
    }
    if (instrumentalPlayer.getAudioData()) {
        if (fabs(instrumentalPlayer.getOriginalTrackDurationInSeconds() - vocalPartPianoPlayer.getOriginalTrackDurationInSeconds()) > 0.005) {
            throw VocalTrainerPlayerPrepareException(VocalTrainerPlayerPrepareException::DIFFERENT_DURATIONS);
        }
    }
    prepareFinishedListeners.executeAll();
    vocalPartChangedListeners.executeAll(&vocalPartPianoPlayer.getVocalPart());
}

const VocalPart * VocalTrainerFilePlayer::getVocalPart() const {
    if (!vocalPartPianoPlayer.isPrepared()) {
        return nullptr;
    }

    return &vocalPartPianoPlayer.getVocalPart();
}

const PlaybackBounds & VocalTrainerFilePlayer::getBounds() const {
    return bounds;
}

void VocalTrainerFilePlayer::setBounds(const PlaybackBounds &bounds) {
    assert(!bounds || (bounds.getStartSeek() >= 0 &&
            bounds.getEndSeek() <= getMainPlayer()->getOriginalTrackDurationInSeconds()));
    if (!isPlaying()) {
        this->bounds = bounds;
        if (bounds) {
            setSeek(bounds.getStartSeek());
        }
        boundsChangedListeners.executeAll(bounds);
    } else {
        isPlayingChangedListeners.addOneShotListener([=] (bool isPlaying) {
            assert(!isPlaying);
            this->setBounds(bounds);
        });
        pause();
    }
}

bool VocalTrainerFilePlayer::isPlaying() const {
    const auto *player = getMainPlayer();
    return player ? player->isPlaying() : false;
}

void VocalTrainerFilePlayer::stopAndMoveSeekToBeginning() {
    isPlayingChangedListeners.addOneShotListener([=] (bool) {
        setSeek(bounds ? bounds.getStartSeek() : 0);
    });
    pause();
}

double VocalTrainerFilePlayer::getSeek() const {
    return getMainPlayer()->getSeek();
}

double VocalTrainerFilePlayer::getPlayStartedSeek() const {
    return playStartedSeek;
}

double VocalTrainerFilePlayer::getPlayStartedTime() const {
    return playStartedTime;
}

double VocalTrainerFilePlayer::getDuration() const {
    return getMainPlayer()->getTrackDurationInSecondsWithTempoApplied();
}

double VocalTrainerFilePlayer::getBeatsPerMinute() const {
    return getOriginalBeatsPerMinute() * getTempoFactor();
}

void VocalTrainerFilePlayer::onPlaybackStarted() {
    playStartedTime = TimeUtils::NowInSeconds();
    playStartedSeek = getSeek();
    isPlayingChangedListeners.executeAll(true);
}

void VocalTrainerFilePlayer::onPlaybackStopped() {
    isPlayingChangedListeners.executeAll(false);
}

bool VocalTrainerFilePlayer::hasPitchNow(const Pitch &pitch) const {
    if (!vocalPartPianoPlayer.isPrepared()) {
        return false;
    }

    return getVocalPart()->hasPitchInMoment(getSeek(), pitch);
}

bool VocalTrainerFilePlayer::hasAnyPitchNow() const {
    if (!vocalPartPianoPlayer.isPrepared()) {
        return false;
    }

    return getVocalPart()->hasPitchesInMoment(getSeek());
}

int VocalTrainerFilePlayer::getPitchShiftInSemiTones() const {
    return vocalPartPianoPlayer.getPitchShiftInSemiTones();
}

void VocalTrainerFilePlayer::setPitchShiftInSemiTones(int value) {
    if (value == getPitchShiftInSemiTones()) {
        return;
    }

    vocalPartPianoPlayer.setPitchShiftInSemiTones(value);
    instrumentalPlayer.setPitchShiftInSemiTones(value);
    recordingPlayer.setPitchShiftInSemiTones(value);
    tonalityChangedListeners.executeAll();
    if (!isRecording()) {
        double seek = getSeek();
        if (seek != 0) {
            tonalityChanges[seek] = value;
        }
    }

    vocalPartChangedListeners.executeAll(&vocalPartPianoPlayer.getVocalPart());
}

bool VocalTrainerFilePlayer::canBeShifted(int distance) const {
    return vocalPartPianoPlayer.isPitchShiftingAvailable(distance);
}

double VocalTrainerFilePlayer::getTempoFactor() const {
    return instrumentalPlayer.getTempoFactor();
}

void VocalTrainerFilePlayer::setTempoFactor(double tempoFactor) {
    vocalPartPianoPlayer.setTempoFactor(tempoFactor);
    instrumentalPlayer.setTempoFactor(tempoFactor);
    metronomePlayer.setAudioDataInfo(getBeatsPerMinute(),
            instrumentalPlayer.getTrackDurationInSecondsWithTempoApplied());
    vocalPartChangedListeners.executeAll(&vocalPartPianoPlayer.getVocalPart());
}

void VocalTrainerFilePlayer::setMetronomeSoundData(std::string &&data) {
    metronomePlayer.setMetronomeAudioData(std::move(data));
    metronomePlayer.prepare();
    players.push_back(&metronomePlayer);
}

bool VocalTrainerFilePlayer::isMetronomeEnabled() const {
    return metronomeEnabled;
}

void VocalTrainerFilePlayer::setMetronomeEnabled(bool metronomeEnabled) {
    assert(!metronomeEnabled || metronomePlayer.isPrepared() && "Metronome data has not been provided, "
                                                                "call setMetronomeSoundData and then prepare, "
                                                                "before enabling metronome");
    this->metronomeEnabled = metronomeEnabled;
    updateMetronomeVolume();
}

bool VocalTrainerFilePlayer::isMetronomeSoundDataSet() const {
    return metronomePlayer.isPrepared();
}

void VocalTrainerFilePlayer::updateMetronomeVolume() {
    if (metronomeEnabled) {
        metronomePlayer.setVolume(std::max({instrumentalPlayer.getVolume(),
                vocalPartPianoPlayer.getVolume(), recordingPlayer.getVolume()}) * 0.6f);
    } else {
        metronomePlayer.setVolume(0.0f);
    }
}

void VocalTrainerFilePlayer::seekToNextTact() {
    double tactDuration = getTactDuration();
    double seek = getSeek() + tactDuration;
    double mod = fmod(seek, tactDuration);
    setSeek(seek - mod);
}

void VocalTrainerFilePlayer::seekToPrevTact() {
    double tactDuration = getTactDuration();
    double seek = getSeek();
    double mod = fmod(seek, tactDuration);
    setSeek(seek - mod);
}

bool VocalTrainerFilePlayer::isRecording() const {
    return file->isRecording();
}

const std::string &VocalTrainerFilePlayer::getArtistNameUtf8() const {
    return file->getArtistNameUtf8();
}

const std::string &VocalTrainerFilePlayer::getSongTitleUtf8() const {
    return file->getSongTitleUtf8();
}

bool VocalTrainerFilePlayer::isCompleted() const {
    return instrumentalPlayer.isCompleted();
}

const PitchesCollection* VocalTrainerFilePlayer::getPitchesCollection() {
    return pitchesCollection;
}

const VocalTrainerFile &VocalTrainerFilePlayer::getFile() const {
    return *file;
}

const BaseAudioPlayer & VocalTrainerFilePlayer::getInstrumentalPlayer() const {
    return instrumentalPlayer;
}

const VocalPartAudioPlayer &VocalTrainerFilePlayer::getVocalPartPlayer() const {
    return vocalPartPianoPlayer;
}

const std::map<double, int> &VocalTrainerFilePlayer::getTonalityChanges() const {
    return tonalityChanges;
}

const BaseAudioPlayer * VocalTrainerFilePlayer::getMainPlayer() const {
    return &instrumentalPlayer;
}

BaseAudioPlayer * VocalTrainerFilePlayer::getMainPlayer() {
    return &instrumentalPlayer;
}

const LyricsDisplayedLinesProvider *VocalTrainerFilePlayer::getDisplayedLyricsLines() const {
    return lyricsPlayer;
}

const std::vector<Lyrics::Section>& VocalTrainerFilePlayer::getLyricsSections() const {
    return file->getLyrics().getSections();
}

double VocalTrainerFilePlayer::getOriginalBeatsPerMinute() const {
    return file->getBeatsPerMinute();
}

void VocalTrainerFilePlayer::setDestroyFileOnDestructor(bool destroyFileOnDestructor) {
    this->destroyFileOnDestructor = destroyFileOnDestructor;
}

void VocalTrainerFilePlayer::clearSource() {
    cancelAllOperations();
    instrumentalPlayer.reset();
    vocalPartPianoPlayer.reset();
    recordingPlayer.reset();
    if (destroyFileOnDestructor) {
        delete file;
    }
    file = nullptr;
}

bool VocalTrainerFilePlayer::hasSource() const {
    return file != nullptr;
}
