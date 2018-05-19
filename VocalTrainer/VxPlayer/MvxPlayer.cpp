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

using namespace CppUtils;

MvxPlayer::MvxPlayer(std::istream &is) {
    init(is);
}

void MvxPlayer::init(std::istream &is) {
    MvxFile file = MvxFile::readFromStream(is);
    instrumentalPlayer = AudioFilePlayer::create(std::move(file.getInstrumental()));
    vxPlayer = new VxFileAudioPlayer(std::move(file.getVxFile()));

    // prevent desynchronization, when some of players have no data ready for playback
    setupVxPlayerDesyncHandler();
    setupInstrumentalPlayerDesyncHandler();

    instrumentalPlayer->addSeekChangedListener([=](double seek, double) {
        if (bounds) {
            if (seek >= bounds->getEndSeek()) {
                pause();
                this->seek(seek);
            }
        }

        return DONT_DELETE_LISTENER;
    });
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

MvxPlayer::MvxPlayer(const char *filePath) {
    std::fstream is = Streams::OpenFile(filePath, std::ios::in | std::ios::binary);
    init(is);
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
            vxPlayer->setSeek(startSeek);
            instrumentalPlayer->setSeek(startSeek);
        }
    }
    
    vxPlayer->play();
    instrumentalPlayer->play();
}

void MvxPlayer::seek(double value) {
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
    vxPlayer->destroy(nullptr);
    vxPlayer = nullptr;
    instrumentalPlayer->destroy();
    instrumentalPlayer = nullptr;
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
    return instrumentalPlayer->isPlaying();
}

void MvxPlayer::stopAndMoveSeekToBeginning() {
    pause();
    seek(bounds ? bounds->getStartSeek() : 0);
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
