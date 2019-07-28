//
// Created by Semyon Tikhonenko on 1/15/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "Executors.h"
#include "MvxPlayer.h"
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
#include <PitchesMutableList.h>
#include "PrepareFailedException.h"

using namespace CppUtils;
using std::cout;
using std::endl;

constexpr int BEATS_IN_TACT = 4;

MvxPlayer::MvxPlayer() : metronomeEnabled(false) {
    instrumentalPlayer.seekChangedListeners.addListener([=](double seek, double) {
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
                const auto& tonalityChanges = mvxFile->getRecordingTonalityChanges();
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
        });

        seekChangedListeners.executeAll(seek);
    });

    instrumentalPlayer.onCompleteListeners.addListener([=] {
        this->onComplete();
        onPlaybackStopped();
    });

    instrumentalPlayer.onPlaybackStartedListeners.addListener([=] {
        this->onPlaybackStarted();
    });
}

void MvxPlayer::setSource(MvxFile *file, bool destroyMvxFileOnDestructor) {
    this->mvxFile = file;
    this->destroyMvxFileOnDestructor = destroyMvxFileOnDestructor;
    instrumentalPlayer.setAudioData(&mvxFile->getInstrumental());
    vocalPartPianoPlayer.setVocalPart(mvxFile->getVocalPart());
    players = {&instrumentalPlayer, &vocalPartPianoPlayer, &metronomePlayer};
    if (mvxFile->isRecording()) {
        pitchesCollection = new PitchesMutableList(mvxFile->getRecordedPitchesFrequencies(),
                                                   mvxFile->getRecordedPitchesTimes());
        recordingPlayer.setAudioData(&mvxFile->getRecordingData());
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
}

void MvxPlayer::setSource(std::istream &is) {
    auto* mvxFile = new MvxFile();
    mvxFile->readFromStream(is);
    this->mvxFile = mvxFile;
    setSource(mvxFile);
}

void MvxPlayer::setSource(const char *filePath) {
    std::fstream is = Streams::OpenFile(filePath, std::ios::in | std::ios::binary);
    setSource(is);
}

void MvxPlayer::onComplete() {
    stopAndMoveSeekToBeginning();
    onCompleteListeners.executeAll();
}

void MvxPlayer::pausePlayer(AudioPlayer *player) {
    Executors::ExecuteOnBackgroundThread([=] {
        player->pauseSync();
        Executors::ExecuteOnMainThread([=] {
            if (--pauseRequestedCounter == 0) {
                onPlaybackStopped();
            }
        });
    });
}

void MvxPlayer::pause() {
    if (!isPlaying() || pauseRequestedCounter != 0) {
        return;
    }

    stopRequestedListeners.executeAll();

    pauseRequestedCounter = players.size();
    for (auto* player : players) {
        pausePlayer(player);
    }
}

void MvxPlayer::play() {
    playRequestedListeners.executeAll();
    updateMetronomeVolume();

    if (bounds) {
        double seekValue = instrumentalPlayer.getSeek();
        if (!bounds.isInside(seekValue)) {
            double startSeek = bounds.getStartSeek();
            instrumentalPlayer.setSeek(startSeek);
        }
    }

    double seek = instrumentalPlayer.getSeek();
    vocalPartPianoPlayer.setSeek(seek);
    if (metronomePlayer.isPrepared()) {
        metronomePlayer.setSeek(seek);
        metronomePlayer.play();
    }
    vocalPartPianoPlayer.play();
    instrumentalPlayer.play();
    if (isRecording()) {
        recordingPlayer.setSeek(seek);
        recordingPlayer.play();
    }
}

void MvxPlayer::setSeek(double value) {
    assert(value >= 0 && value <= instrumentalPlayer.getTrackDurationInSeconds());
    for (auto* player : players) {
        player->setSeek(value);
    }
    seekChangedFromUserListeners.executeAll(value);
}

void MvxPlayer::setInstrumentalVolume(float instrumentalVolume) {
    instrumentalPlayer.setVolume(instrumentalVolume);
    updateMetronomeVolume();
}

void MvxPlayer::setVocalPartPianoVolume(float pianoVolume) {
    vocalPartPianoPlayer.setVolume(pianoVolume);
    updateMetronomeVolume();
}

float MvxPlayer::getInstrumentalVolume() const {
    return instrumentalPlayer.getVolume();
}

float MvxPlayer::getVocalPianoVolume() const {
    return vocalPartPianoPlayer.getVolume();
}

void MvxPlayer::setRecordingVolume(float volume) {
    recordingPlayer.setVolume(volume);
    updateMetronomeVolume();
}

MvxPlayer::~MvxPlayer() {
    delete pitchesCollection;
    delete recordingLevelMonitor;
    if (destroyMvxFileOnDestructor) {
        delete mvxFile;
    }
}

void MvxPlayer::prepare() {
    try {
        instrumentalPlayer.prepare();
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
        metronomePlayer.setAudioDataInfo(getBeatsPerMinute(), instrumentalPlayer.getTrackDurationInSeconds());
    }
    if (fabs(instrumentalPlayer.getTrackDurationInSeconds() - vocalPartPianoPlayer.getTrackDurationInSeconds()) > 0.005) {
        throw MvxPlayerPrepareException(MvxPlayerPrepareException::DIFFERENT_DURATIONS);
    }
    prepareFinishedListeners.executeAll();
    vocalPartChangedListeners.executeAll(&vocalPartPianoPlayer.getVocalPart());
}

const VocalPart * MvxPlayer::getVocalPart() const {
    if (!vocalPartPianoPlayer.isPrepared()) {
        return nullptr;
    }

    return &vocalPartPianoPlayer.getVocalPart();
}

const PlaybackBounds & MvxPlayer::getBounds() const {
    return bounds;
}

void MvxPlayer::setBounds(const PlaybackBounds &bounds) {
    assert(!bounds || (bounds.getStartSeek() >= 0 &&
            bounds.getEndSeek() <= instrumentalPlayer.getTrackDurationInSeconds()));
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

bool MvxPlayer::isPlaying() const {
    return instrumentalPlayer.isPlaying();
}

void MvxPlayer::stopAndMoveSeekToBeginning() {
    pause();
    setSeek(bounds ? bounds.getStartSeek() : 0);
}

double MvxPlayer::getSeek() const {
    return instrumentalPlayer.getSeek();
}

double MvxPlayer::getPlayStartedSeek() const {
    return playStartedSeek;
}

double MvxPlayer::getPlayStartedTime() const {
    return playStartedTime;
}

double MvxPlayer::getDuration() const {
    return instrumentalPlayer.getTrackDurationInSeconds();
}

double MvxPlayer::getBeatsPerMinute() const {
    return mvxFile->getBeatsPerMinute();
}

double MvxPlayer::getBeatsPerSecond() const {
    return getBeatsPerMinute() / 60.0;
}

double MvxPlayer::getTactsPerSecond() const {
    return getBeatsPerSecond() / BEATS_IN_TACT;
}

void MvxPlayer::onPlaybackStarted() {
    playStartedTime = TimeUtils::NowInSeconds();
    playStartedSeek = getSeek();
    isPlayingChangedListeners.executeAll(true);
}

void MvxPlayer::onPlaybackStopped() {
    isPlayingChangedListeners.executeAll(false);
}

bool MvxPlayer::hasPitchNow(const Pitch &pitch) const {
    if (!vocalPartPianoPlayer.isPrepared()) {
        return false;
    }

    return getVocalPart()->hasPitchInMoment(getSeek(), pitch);
}

bool MvxPlayer::hasAnyPitchNow() const {
    if (!vocalPartPianoPlayer.isPrepared()) {
        return false;
    }

    return getVocalPart()->hasPitchesInMoment(getSeek());
}

int MvxPlayer::getPitchShiftInSemiTones() const {
    return vocalPartPianoPlayer.getPitchShiftInSemiTones();
}

void MvxPlayer::setPitchShiftInSemiTones(int value) {
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

bool MvxPlayer::canBeShifted(int distance) const {
    return vocalPartPianoPlayer.isPitchShiftingAvailable(distance);
}

double MvxPlayer::getTempoFactor() const {
    return instrumentalPlayer.getTempoFactor();
}

void MvxPlayer::setTempoFactor(double tempoFactor) {
    vocalPartPianoPlayer.setTempoFactor(tempoFactor);
    instrumentalPlayer.setTempoFactor(tempoFactor);
}

void MvxPlayer::setMetronomeSoundData(std::string &&data) {
    metronomePlayer.setMetronomeAudioData(std::move(data));
    metronomePlayer.prepare();
}

bool MvxPlayer::isMetronomeEnabled() const {
    return metronomeEnabled;
}

void MvxPlayer::setMetronomeEnabled(bool metronomeEnabled) {
    assert(!metronomeEnabled || metronomePlayer.isPrepared() && "Metronome data has not been provided, "
                                                                "call setMetronomeSoundData and then prepare, "
                                                                "before enabling metronome");
    this->metronomeEnabled = metronomeEnabled;
    updateMetronomeVolume();
}

double MvxPlayer::getBeatDuration() const {
    return 60.0 / mvxFile->getBeatsPerMinute();
}

bool MvxPlayer::isMetronomeSoundDataSet() const {
    return metronomePlayer.isPrepared();
}

void MvxPlayer::updateMetronomeVolume() {
    if (metronomeEnabled) {
        metronomePlayer.setVolume(std::max({instrumentalPlayer.getVolume(),
                vocalPartPianoPlayer.getVolume(), recordingPlayer.getVolume()}) * 1.f);
    } else {
        metronomePlayer.setVolume(0.0f);
    }
}

void MvxPlayer::seekToNextTact() {
    double tactDuration = getTactDuration();
    double seek = getSeek() + tactDuration;
    double mod = fmod(seek, tactDuration);
    setSeek(seek - mod);
}

void MvxPlayer::seekToPrevTact() {
    double tactDuration = getTactDuration();
    double seek = getSeek();
    double mod = fmod(seek, tactDuration);
    setSeek(seek - mod);
}

double MvxPlayer::getTactDuration() const {
    return getBeatDuration() * BEATS_IN_TACT;
}

bool MvxPlayer::isRecording() const {
    return mvxFile->isRecording();
}

const std::string &MvxPlayer::getArtistNameUtf8() const {
    return mvxFile->getArtistNameUtf8();
}

const std::string &MvxPlayer::getSongTitleUtf8() const {
    return mvxFile->getSongTitleUtf8();
}

const std::string &MvxPlayer::getInstrumental() {
    return mvxFile->getInstrumental();
}

bool MvxPlayer::isCompleted() const {
    return instrumentalPlayer.isCompleted();
}

const PitchesCollection* MvxPlayer::getPitchesCollection() {
    return pitchesCollection;
}

const MvxFile &MvxPlayer::getMvxFile() const {
    return *mvxFile;
}

bool MvxPlayer::hasLyrics() const {
    return getLyricsLinesCount() > 0;
}

int MvxPlayer::getLyricsLinesCount() const {
    return mvxFile->getLyrics().getLinesCount();
}

const std::string& MvxPlayer::getLyricsTextAtLine(int lineIndex) const {
    double seek = getSeek();
    return mvxFile->getLyrics().getCurrentLyricsTextAtLine(lineIndex, seek);
}

const AudioPlayer &MvxPlayer::getInstrumentalPlayer() const {
    return instrumentalPlayer;
}

const VocalPartAudioPlayer &MvxPlayer::getVocalPartPlayer() const {
    return vocalPartPianoPlayer;
}

const std::map<double, int> &MvxPlayer::getTonalityChanges() const {
    return tonalityChanges;
}

void MvxPlayer::editLyrics(const std::function<void(Lyrics *lyrics)> &editAction) {
    auto lyrics = mvxFile->getLyrics();
    editAction(&mvxFile->getLyricsNonConst());
    if (lyrics != mvxFile->getLyrics()) {
        lyricsChangedListeners.executeAll();
    }
}
