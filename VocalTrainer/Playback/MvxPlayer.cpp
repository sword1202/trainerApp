//
// Created by Semyon Tikhonenko on 1/15/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <Executors.h>
#include "MvxPlayer.h"
#include "StringUtils.h"
#include "AudioFilePlayer.h"
#include "VxFileAudioPlayer.h"
#include "MvxFile.h"
#include "Primitives.h"
#include "TimeUtils.h"
#include "Functions.h"
#include "Executors.h"

using namespace CppUtils;

MvxPlayer::MvxPlayer() {
    instrumentalPlayer.addSeekChangedListener([=](double seek, double) {
        seekChangedListeners.executeAll(seek);

        if (bounds) {
            if (seek >= bounds->getEndSeek()) {
                Executors::ExecuteOnMainThread([this] {this->onComplete();});
            }
        }

        Executors::ExecuteOnMainThread([this, seek] {
            this->onSeekChanged(seek);
        });

        return DONT_DELETE_LISTENER;
    });

    instrumentalPlayer.addOnCompleteListener([=] {
        Executors::ExecuteOnMainThread([this] {this->onComplete();});
        return DONT_DELETE_LISTENER;
    });

    instrumentalPlayer.addPlaybackStartedListener([=] {
        Executors::ExecuteOnMainThread([=] {this->onPlaybackStarted();});
        return DONT_DELETE_LISTENER;
    });

    instrumentalPlayer.addPlaybackStoppedListener([=] {
        Executors::ExecuteOnMainThread([this] {this->onPlaybackStopped();});
        return DONT_DELETE_LISTENER;
    });
}

void MvxPlayer::init(std::istream &is) {
    MvxFile file = MvxFile::readFromStream(is);
    instrumentalPlayer.setAudioData(std::move(file.getInstrumental()));
    vxPlayer.setVxFile(file.getVxFile());

    beatsPerMinute = file.getBeatsPerMinute();
}

void MvxPlayer::init(const char *filePath) {
    std::fstream is = Streams::OpenFile(filePath, std::ios::in | std::ios::binary);
    init(is);
}

void MvxPlayer::onComplete() {
    stopAndMoveSeekToBeginning();
}

void MvxPlayer::pause() {
    instrumentalPlayer.pause();
    vxPlayer.pause();
}

void MvxPlayer::play() {
    if (bounds) {
        double seekValue = instrumentalPlayer.getSeek();
        if (!bounds->isInside(seekValue)) {
            double startSeek = bounds->getStartSeek();
            instrumentalPlayer.setSeek(startSeek);
        }
    }

    double seek = instrumentalPlayer.getSeek();
    vxPlayer.setSeek(seek);
    vxPlayer.play();
    instrumentalPlayer.play();
}

void MvxPlayer::setSeek(double value) {
    assert(!bounds || (bounds->getStartSeek() <= value &&
            bounds->getEndSeek() <= value));
    assert(value >= 0 && value <= instrumentalPlayer.getTrackDurationInSeconds());
    instrumentalPlayer.setSeek(value);
    vxPlayer.setSeek(value);
}

void MvxPlayer::setInstrumentalVolume(float instrumentalVolume) {
    instrumentalPlayer.setVolume(instrumentalVolume);
}

void MvxPlayer::setPianoVolume(float pianoVolume) {
    vxPlayer.setVolume(pianoVolume);
}

MvxPlayer::~MvxPlayer() {

}

void MvxPlayer::prepare() {
    instrumentalPlayer.prepare();
    vxPlayer.prepare();
    assert(fabs(instrumentalPlayer.getTrackDurationInSeconds() - vxPlayer.getTrackDurationInSeconds()) < 0.1);
    prepareFinishedListeners.executeAll();
    vxFileChangedListeners.executeAll(&vxPlayer.getVxFile());
}

const VxFile * MvxPlayer::getVxFile() const {
    if (vxPlayer.isPrepared()) {
        return nullptr;
    }

    return &vxPlayer.getVxFile();
}

const boost::optional<MvxPlayer::Bounds> &MvxPlayer::getBounds() const {
    return bounds;
}

void MvxPlayer::setBounds(const boost::optional<MvxPlayer::Bounds> &bounds) {
    assert(!instrumentalPlayer.isPlaying());
    assert(!bounds || (bounds->getStartSeek() >= 0 &&
            bounds->getEndSeek() <= instrumentalPlayer.getTrackDurationInSeconds()));
    this->bounds = bounds;
}

bool MvxPlayer::isPlaying() const {
    return instrumentalPlayer.isPlaying();
}

void MvxPlayer::stopAndMoveSeekToBeginning() {
    pause();
    setSeek(bounds ? bounds->getStartSeek() : 0);
}

double MvxPlayer::getSeek() const {
    return instrumentalPlayer.getSeek();
}

void MvxPlayer::onSeekChanged(double seek) {

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
    return beatsPerMinute;
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

int MvxPlayer::addIsPlayingChangedListener(const MvxPlayer::IsPlayingChangedListener &listener) {
    return isPlayingChangedListeners.addListener(listener);
}

void MvxPlayer::removeIsPlayingChangedListener(int id) {
    isPlayingChangedListeners.removeListener(id);
}

int MvxPlayer::addPrepareFinishedListener(const MvxPlayer::PrepareFinishedListener &listener) {
    return prepareFinishedListeners.addListener(listener);
}

void MvxPlayer::removePrepareFinishedListener(int id) {
    prepareFinishedListeners.removeListener(id);
}

int MvxPlayer::addVxFileChangedListener(const MvxPlayer::VxFileChangedListener &listener) {
    return vxFileChangedListeners.addListener(listener);
}

void MvxPlayer::removeVxFileChangedListener(int id) {
    vxFileChangedListeners.removeListener(id);
}

int MvxPlayer::addSeekChangedListener(const SeekChangedListener& listener) {
    return seekChangedListeners.addListener(listener);
}

void MvxPlayer::removeSeekChangedListener(int id) {
    seekChangedListeners.removeListener(id);
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

int MvxPlayer::addTonalityChangedListener(const MvxPlayer::TonalityChangedListener &listener) {
    return tonalityChangedListeners.addListener(listener);
}

void MvxPlayer::removeTonalityChangedListener(int id) {
    tonalityChangedListeners.removeListener(id);
}

MvxPlayer::Bounds::Bounds(double startSeek, double endSeek) : startSeek(startSeek), endSeek(endSeek) {
}

double MvxPlayer::Bounds::getStartSeek() const {
    return startSeek;
}

double MvxPlayer::Bounds::getEndSeek() const {
    return endSeek;
}

bool MvxPlayer::Bounds::isInside(double value) const {
    return value >= startSeek && value <= endSeek;
}

bool MvxPlayer::Bounds::operator==(const MvxPlayer::Bounds &rhs) const {
    return startSeek == rhs.startSeek &&
            endSeek == rhs.endSeek;
}

bool MvxPlayer::Bounds::operator!=(const MvxPlayer::Bounds &rhs) const {
    return !(rhs == *this);
}
