//
// Created by Semyon Tikhonenko on 1/15/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "Executors.h"
#include "MvxPlayer.h"
#include "StringUtils.h"
#include "AudioFilePlayer.h"
#include "VxFileAudioPlayer.h"
#include "MvxFile.h"
#include "Primitives.h"
#include "TimeUtils.h"
#include "Functions.h"
#include "Executors.h"
#include "PlaybackBounds.h"
#include <iostream>
#include <memory>

using namespace CppUtils;
using std::cout;
using std::endl;

constexpr int BEATS_IN_TACT = 4;

MvxPlayer::MvxPlayer() : metronomeEnabled(false) {
    players = {{&instrumentalPlayer, &vxPlayer, &metronomePlayer}};

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

void MvxPlayer::init(std::istream &is) {
    mvxFile = MvxFile::readFromStream(is);
    instrumentalPlayer.setAudioData(std::move(mvxFile.moveInstrumental()));
    vxPlayer.setVxFile(mvxFile.getVxFile());
}

void MvxPlayer::init(const char *filePath) {
    std::fstream is = Streams::OpenFile(filePath, std::ios::in | std::ios::binary);
    init(is);
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
    vxPlayer.setSeek(seek);
    if (metronomePlayer.isPrepared()) {
        metronomePlayer.setSeek(seek);
        metronomePlayer.play();
    }
    vxPlayer.play();
    instrumentalPlayer.play();
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

void MvxPlayer::setPianoVolume(float pianoVolume) {
    vxPlayer.setVolume(pianoVolume);
    updateMetronomeVolume();
}

MvxPlayer::~MvxPlayer() {

}

void MvxPlayer::prepare() {
    instrumentalPlayer.prepare();
    vxPlayer.prepare();
    metronomePlayer.setAudioDataInfo(getBeatsPerMinute(), instrumentalPlayer.getTrackDurationInSeconds());
    assert(fabs(instrumentalPlayer.getTrackDurationInSeconds() - vxPlayer.getTrackDurationInSeconds()) < 0.1);
    prepareFinishedListeners.executeAll();
    vxFileChangedListeners.executeAll(&vxPlayer.getVxFile());
}

const VxFile * MvxPlayer::getVxFile() const {
    if (!vxPlayer.isPrepared()) {
        return nullptr;
    }

    return &vxPlayer.getVxFile();
}

const PlaybackBounds & MvxPlayer::getBounds() const {
    return bounds;
}

void MvxPlayer::setBounds(const PlaybackBounds &bounds) {
    assert(!instrumentalPlayer.isPlaying());
    assert(!bounds || (bounds.getStartSeek() >= 0 &&
            bounds.getEndSeek() <= instrumentalPlayer.getTrackDurationInSeconds()));
    this->bounds = bounds;
    boundsChangedListeners.executeAll(bounds);
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
    return mvxFile.getBeatsPerMinute();
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
    pauseRequestedCounter = false;
    isPlayingChangedListeners.executeAll(false);
}

bool MvxPlayer::hasPitchNow(const Pitch &pitch) const {
    if (!vxPlayer.isPrepared()) {
        return false;
    }

    return getVxFile()->hasPitchInMoment(getSeek(), pitch);
}

bool MvxPlayer::hasAnyPitchNow() const {
    if (!vxPlayer.isPrepared()) {
        return false;
    }

    return getVxFile()->hasPitchesInMoment(getSeek());
}

int MvxPlayer::getPitchShiftInSemiTones() const {
    return vxPlayer.getPitchShiftInSemiTones();
}

void MvxPlayer::setPitchShiftInSemiTones(int value) {
    vxPlayer.setPitchShiftInSemiTones(value);
    instrumentalPlayer.setPitchShiftInSemiTones(value);
    tonalityChangedListeners.executeAll();
}

bool MvxPlayer::canBeShifted(int distance) const {
    return vxPlayer.isPitchShiftingAvailable(distance);
}

double MvxPlayer::getTempoFactor() const {
    return instrumentalPlayer.getTempoFactor();
}

void MvxPlayer::setTempoFactor(double tempoFactor) {
    vxPlayer.setTempoFactor(tempoFactor);
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
    this->metronomeEnabled = metronomeEnabled;
    updateMetronomeVolume();
}

double MvxPlayer::getBeatDuration() const {
    return 60.0 / mvxFile.getBeatsPerMinute();
}

bool MvxPlayer::isMetronomeSoundDataSet() const {
    return metronomePlayer.isPrepared();
}

void MvxPlayer::updateMetronomeVolume() {
    if (metronomeEnabled) {
        metronomePlayer.setVolume(std::max(instrumentalPlayer.getVolume(), vxPlayer.getVolume()) * 0.2f);
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
    return mvxFile.isRecording();
}

const std::string &MvxPlayer::getArtistNameUtf8() const {
    return mvxFile.getArtistNameUtf8();
}

const std::string &MvxPlayer::getSongTitleUtf8() const {
    return mvxFile.getSongTitleUtf8();
}
