//
// Created by Semyon Tikhonenko on 1/15/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MvxPlayer.h"
#include "Strings.h"
#include "AudioFilePlayer.h"
#include "VxFileAudioPlayer.h"
#include "MvxFile.h"

constexpr float MAX_PIANO_VOLUME = 1.0f;

using namespace CppUtils;

MvxPlayer::MvxPlayer(std::istream &is) {
    init(is);
}

void MvxPlayer::init(std::istream &is) {
    MvxFile file = MvxFile::readFromStream(is);
    instrumentalPlayer = AudioFilePlayer::create(std::move(file.getInstrumental()));
    vxPlayer = new VxFileAudioPlayer(std::move(file.getVxFile()));
}

MvxPlayer::MvxPlayer(const char *filePath) {
    std::fstream is = Streams::OpenFile(filePath, std::ios::in | std::ios::binary);
    init(is);
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
    vxPlayer->destroy();
    vxPlayer = nullptr;
    instrumentalPlayer->destroy();
    instrumentalPlayer = nullptr;
}

void MvxPlayer::prepare() {
    instrumentalPlayer->prepare();
    vxPlayer->prepare();
}

MvxPlayer::MvxPlayer(MvxFile &&mvxFile) {
    instrumentalPlayer = AudioFilePlayer::create(std::move(mvxFile.getInstrumental()));
    vxPlayer = new VxFileAudioPlayer(std::move(mvxFile.getVxFile()));
}
