//
// Created by Semyon Tikhonenko on 1/15/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MvxPlayer.h"
#include "Strings.h"
#include "AudioFilePlayer.h"
#include "VxFileAudioPlayer.h"

constexpr float MAX_PIANO_VOLUME = 1.0f;

using namespace CppUtils;

MvxPlayer::MvxPlayer(std::istream &is) {
    init(is);
}

void MvxPlayer::init(std::istream &is) {
    vxFile = new VxFile(is);
    destroyVxFileOnDestructor = true;
    // skip space;
    is.get();
    instrumentalPlayer = AudioFilePlayer::create(is);
    vxPlayer = new VxFileAudioPlayer(vxFile);
}

MvxPlayer::MvxPlayer(const char *filePath) {
    std::fstream is = Streams::OpenFile(filePath, std::ios::in | std::ios::binary);
    init(is);
}

MvxPlayer::MvxPlayer(const char *instrumentalFilePath, const VxFile *vxFile) {
    std::fstream is = Streams::OpenFile(instrumentalFilePath, std::ios::in | std::ios::binary);
    init(is, vxFile);
}

MvxPlayer::MvxPlayer(std::istream &instrumentalStream, const VxFile *vxFile) {
    init(instrumentalStream, vxFile);
}

void MvxPlayer::init(std::istream &instrumentalStream, const VxFile* vxFile) {
    this->vxFile = vxFile;
}

void MvxPlayer::play(float instrumentalVolume, float pianoVolume) {
    vxPlayer->setVolume(pianoVolume);
    instrumentalPlayer->setVolume(instrumentalVolume);
    vxPlayer->play();
    instrumentalPlayer->play();
}

void MvxPlayer::pause() {
    instrumentalPlayer->pause();
    vxPlayer->pause();
}

void MvxPlayer::resume() {
    vxPlayer->play();
    instrumentalPlayer->play();
}

void MvxPlayer::seek(double value) {
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
    delete vxPlayer;
    delete instrumentalPlayer;
    if (destroyVxFileOnDestructor) {
        delete vxFile;
    }
}

void MvxPlayer::prepare() {
    instrumentalPlayer->prepare();
    vxPlayer->prepare();
}