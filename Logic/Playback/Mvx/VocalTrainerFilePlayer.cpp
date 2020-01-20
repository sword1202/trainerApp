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
#include "MvxPlayerPrepareException.h"
#include <iostream>
#include <memory>
#include "PitchesMutableList.h"
#include "PrepareFailedException.h"

using namespace CppUtils;
using std::cout;
using std::endl;

constexpr int BEATS_IN_TACT = 4;

VocalTrainerFilePlayer::VocalTrainerFilePlayer() : metronomeEnabled(false) {

}

void VocalTrainerFilePlayer::setSource(VocalTrainerFile *file, bool destroyFileOnDestructor) {
    this->file = file;
    this->destroyMvxFileOnDestructor = destroyFileOnDestructor;
    players.clear();
    const AudioData* instrumental = &file->getInstrumental();
    if (!instrumental->empty()) {
        instrumentalPlayer.setAudioData(instrumental);
        players.push_back(&instrumentalPlayer);
    } else {
        instrumentalPlayer.setAudioData(nullptr);
    }

    vocalPartPianoPlayer.setVocalPart(file->getVocalPart());
    players.push_back(&vocalPartPianoPlayer);
    if (file->isRecording()) {
        pitchesCollection = new PitchesMutableList(file->getRecordedPitchesFrequencies(),
                                                   file->getRecordedPitchesTimes());
        recordingPlayer.setAudioData(&file->getRecordingData());
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

    AudioPlayer* mainPlayer = getMainPlayer();
    mainPlayer->seekChangedListeners.clear();
    mainPlayer->onCompleteListeners.clear();
    mainPlayer->onPlaybackStartedListeners.clear();
    mainPlayer->seekChangedListeners.addListener([=](double seek, double) {
        Executors::ExecuteOnMainThread([=] {
            if (bounds) {
                if (seek >= bounds.getEndSeek()) {
                    // avoid multiple onComplete calls
                    if (pauseRequestedCounter == 0) {
                        this->onComplete();
                    }
                }
            }

            if (isRecording()) {
                const auto& tonalityChanges = file->getRecordingTonalityChanges();
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

            auto currentSnapshot = file->getLyrics().getCurrentSnapshot(seek);
            if (lastLyricsSnapshot != currentSnapshot) {
                lyricsChangedListeners.executeAll();
                lastLyricsSnapshot = currentSnapshot;
            }
        });

        seekChangedListeners.executeAll(seek);
    });

    mainPlayer->onCompleteListeners.addListener([=] {
        this->onComplete();
        onPlaybackStopped();
    });

    mainPlayer->onPlaybackStartedListeners.addListener([=] {
        this->onPlaybackStarted();
    });
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
    stopAndMoveSeekToBeginning();
    onCompleteListeners.executeAll();
}

void VocalTrainerFilePlayer::pausePlayer(AudioPlayer *player) {
    Executors::ExecuteOnBackgroundThread([=] {
        player->pauseSync();
        Executors::ExecuteOnMainThread([=] {
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

    AudioPlayer* mainPlayer = getMainPlayer();
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

    for (AudioPlayer* player : players) {
        player->play();
    }
}

void VocalTrainerFilePlayer::setSeek(double value) {
    assert(value >= 0 && value <= getMainPlayer()->getTrackDurationInSeconds());
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
    if (destroyMvxFileOnDestructor) {
        delete file;
    }
}

void VocalTrainerFilePlayer::prepare() {
    try {
        if (instrumentalPlayer.getAudioData() != nullptr) {
            instrumentalPlayer.prepare();
        }
    } catch (PrepareFailedException&) {
        throw MvxPlayerPrepareException(MvxPlayerPrepareException::BROKEN_INSTRUMENTAL);
    }
    try {
        vocalPartPianoPlayer.prepare();
    } catch (PrepareFailedException&) {
        throw MvxPlayerPrepareException(MvxPlayerPrepareException::BROKEN_VOCAL_PART);
    }
    if (isRecording()) {
        try {
            recordingPlayer.prepare();
        } catch (PrepareFailedException&) {
            throw MvxPlayerPrepareException(MvxPlayerPrepareException::BROKEN_RECORDING);
        }
    }

    if (metronomePlayer.isPrepared()) {
        metronomePlayer.setAudioDataInfo(getBeatsPerMinute(), getMainPlayer()->getTrackDurationInSeconds());
    }
    if (instrumentalPlayer.getAudioData()) {
        if (fabs(instrumentalPlayer.getTrackDurationInSeconds() - vocalPartPianoPlayer.getTrackDurationInSeconds()) > 0.005) {
            throw MvxPlayerPrepareException(MvxPlayerPrepareException::DIFFERENT_DURATIONS);
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
            bounds.getEndSeek() <= getMainPlayer()->getTrackDurationInSeconds()));
    if (!isPlaying()) {
        this->bounds = bounds;
        boundsChangedListeners.executeAll(bounds);
    } else {
        isPlayingChangedListeners.addOneShotListener([=] (bool isPlaying) {
            assert(!isPlaying);
            if (bounds) {
                setSeek(bounds.getStartSeek());
            }
            this->setBounds(bounds);
        });
        pause();
    }
}

bool VocalTrainerFilePlayer::isPlaying() const {
    return getMainPlayer()->isPlaying();
}

void VocalTrainerFilePlayer::stopAndMoveSeekToBeginning() {
    pause();
    setSeek(bounds ? bounds.getStartSeek() : 0);
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
    return getMainPlayer()->getTrackDurationInSeconds();
}

double VocalTrainerFilePlayer::getBeatsPerMinute() const {
    return file->getBeatsPerMinute();
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
                vocalPartPianoPlayer.getVolume(), recordingPlayer.getVolume()}) * 1.f);
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

const std::string &VocalTrainerFilePlayer::getInstrumental() {
    return file->getInstrumental();
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

bool VocalTrainerFilePlayer::hasLyrics() const {
    return getLyricsLinesCount() > 0;
}

int VocalTrainerFilePlayer::getLyricsLinesCount() const {
    return file->getLyrics().getNumberOfParts();
}

const std::string& VocalTrainerFilePlayer::getLyricsTextForPart(int partIndex) const {
    double seek = getSeek();
    return file->getLyrics().getCurrentLyricsTextForPart(partIndex, seek);
}

const AudioPlayer &VocalTrainerFilePlayer::getInstrumentalPlayer() const {
    return instrumentalPlayer;
}

const VocalPartAudioPlayer &VocalTrainerFilePlayer::getVocalPartPlayer() const {
    return vocalPartPianoPlayer;
}

const std::map<double, int> &VocalTrainerFilePlayer::getTonalityChanges() const {
    return tonalityChanges;
}

void VocalTrainerFilePlayer::editLyrics(const std::function<void(Lyrics *lyrics)> &editAction) {
    assert(file->isLyricsEditAvailable());
    auto lyrics = file->getLyrics();
    editAction(&file->getLyricsNonConst());
    if (lyrics != file->getLyrics()) {
        lyricsChangedListeners.executeAll();
    }
}

const AudioPlayer* VocalTrainerFilePlayer::getMainPlayer() const {
    return players.at(0);
}

AudioPlayer* VocalTrainerFilePlayer::getMainPlayer() {
    return players.at(0);
}
