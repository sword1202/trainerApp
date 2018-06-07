//
// Created by Semyon Tikhonenko on 1/15/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MvxPlayer.h"
#include "Strings.h"
#include "AudioFilePlayer.h"
#include "VxFileAudioPlayer.h"
#include "MvxFile.h"
#include "Primitives.h"
#include "TimeUtils.h"

using namespace CppUtils;

void MvxPlayer::init(std::istream &is) {
    MvxFile file = MvxFile::readFromStream(is);
    instrumentalPlayer.reset(new AudioFilePlayer(std::move(file.getInstrumental())));
    vxPlayer.reset(new VxFileAudioPlayer(std::move(file.getVxFile())));

    beatsPerMinute = file.getBeatsPerMinute();

    // prevent desynchronization, when some of players have no data ready for playback
    setupVxPlayerDesyncHandler();
    setupInstrumentalPlayerDesyncHandler();

    instrumentalPlayer->addSeekChangedListener([=](double seek, double) {
        if (bounds) {
            if (seek >= bounds->getEndSeek()) {
                onComplete();
            }
        }

        onSeekChanged(getSeek());

        return DONT_DELETE_LISTENER;
    });

    instrumentalPlayer->addOnCompleteListener([=] {
        onComplete();
        return DONT_DELETE_LISTENER;
    });

    instrumentalPlayer->addPlaybackStartedListener([=] {
        onPlaybackStarted();
        return DONT_DELETE_LISTENER;
    });

    instrumentalPlayer->addPlaybackStoppedListener([=] {
        onPlaybackStopped();
        return DONT_DELETE_LISTENER;
    });
}

void MvxPlayer::init(const char *filePath) {
    std::fstream is = Streams::OpenFile(filePath, std::ios::in | std::ios::binary);
    init(is);
}

void MvxPlayer::onComplete() {
    stopAndMoveSeekToBeginning();
}

void MvxPlayer::setupInstrumentalPlayerDesyncHandler() const {
    instrumentalPlayer->addOnNoDataAvailableListener([=] {
        vxPlayer->pause();
        instrumentalPlayer->addOnDataSentToOutputListener([=] (void*, int) {
            vxPlayer->play(instrumentalPlayer->getSeek());
            setupInstrumentalPlayerDesyncHandler();
            return DELETE_LISTENER;
        });

        return DELETE_LISTENER;
    });
}

void MvxPlayer::setupVxPlayerDesyncHandler() const {
    vxPlayer->addOnNoDataAvailableListener([=] {
        instrumentalPlayer->pause();
        vxPlayer->addOnDataSentToOutputListener([=] (void*, int) {
            instrumentalPlayer->play(vxPlayer->getSeek());
            setupVxPlayerDesyncHandler();
            return DELETE_LISTENER;
        });

        return DELETE_LISTENER;
    });
}

void MvxPlayer::pause() {
    instrumentalPlayer->pause();
    vxPlayer->pause();
}

void MvxPlayer::play() {
    if (bounds) {
        double seekValue = instrumentalPlayer->getSeek();
        if (!bounds->isInside(seekValue)) {
            double startSeek = bounds->getStartSeek();
            instrumentalPlayer->setSeek(startSeek);
        }
    }

    double seek = instrumentalPlayer->getSeek();
    vxPlayer->setSeek(seek);
    vxPlayer->play();
    instrumentalPlayer->play();
}

void MvxPlayer::setSeek(double value) {
    assert(!bounds || (bounds->getStartSeek() <= value &&
            bounds->getEndSeek() <= value));
    assert(value >= 0 && value <= instrumentalPlayer->getTrackDurationInSeconds());
    instrumentalPlayer->setSeek(value);
    vxPlayer->setSeek(value);
}

void MvxPlayer::setInstrumentalVolume(float instrumentalVolume) {
    instrumentalPlayer->setVolume(instrumentalVolume);
}

void MvxPlayer::setPianoVolume(float pianoVolume) {
    vxPlayer->setVolume(pianoVolume);
}

MvxPlayer::~MvxPlayer() {

}

void MvxPlayer::prepare() {
    instrumentalPlayer->prepare();
    vxPlayer->prepare();
    assert(fabs(instrumentalPlayer->getTrackDurationInSeconds() - vxPlayer->getTrackDurationInSeconds()) < 0.1);
}

const VxFile &MvxPlayer::getVxFile() const {
    return vxPlayer->getVxFile();
}

const boost::optional<MvxPlayer::Bounds> &MvxPlayer::getBounds() const {
    return bounds;
}

void MvxPlayer::setBounds(const boost::optional<MvxPlayer::Bounds> &bounds) {
    assert(!instrumentalPlayer->isPlaying());
    assert(!bounds || (bounds->getStartSeek() >= 0 &&
            bounds->getEndSeek() <= instrumentalPlayer->getTrackDurationInSeconds()));
    this->bounds = bounds;
}

bool MvxPlayer::isPlaying() const {
    if (!instrumentalPlayer) {
        return false;
    }

    return instrumentalPlayer->isPlaying();
}

void MvxPlayer::stopAndMoveSeekToBeginning() {
    pause();
    setSeek(bounds ? bounds->getStartSeek() : 0);
}

double MvxPlayer::getSeek() const {
    return instrumentalPlayer->getSeek();
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
    if (!instrumentalPlayer) {
        return -1;
    }

    return instrumentalPlayer->getTrackDurationInSeconds();
}

double MvxPlayer::getBeatsPerMinute() const {
    return beatsPerMinute;
}

void MvxPlayer::onPlaybackStarted() {
    playStartedTime = TimeUtils::NowInSeconds();
    playStartedSeek = getSeek();
}

void MvxPlayer::onPlaybackStopped() {

}

bool MvxPlayer::hasPitchNow(const Pitch &pitch) const {
    if (!vxPlayer) {
        return false;
    }

    return getVxFile().hasPitchInMoment(getSeek(), pitch);
}

bool MvxPlayer::hasAnyPitchNow() const {
    if (!vxPlayer) {
        return false;
    }

    return getVxFile().hasPitchesInMoment(getSeek());
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
