//
// Created by Semyon Tikhonenko on 1/15/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MvxPlayer.h"
#include "Strings.h"

constexpr float MAX_PIANO_VOLUME = 1.0f;

using namespace CppUtils;

void MvxPlayer::loadFromStream(std::istream &is) {
    VxFile vxFile(is);
    // skip space;
    is.get();
    initFromInstrumentalStreamAndVxFile(is, vxFile);
}

void MvxPlayer::initFromInstrumentalFileAndVxFile(const char *instrumentalFilePath, const VxFile& vxFile) {
    std::fstream file(instrumentalFilePath, std::ios::binary | std::ios::in);
    initFromInstrumentalStreamAndVxFile(file, vxFile);
}

void MvxPlayer::initFromInstrumentalStreamAndVxFile(std::istream &instrumentalStream, const VxFile& vxFile) {
    audioData = Strings::StreamToString(instrumentalStream);
    //vxAudioData = vxFile.generateWavAudioData(MAX_PIANO_VOLUME);
}

void MvxPlayer::loadFromFile(const char *filePath) {
    std::fstream file(filePath, std::ios::binary | std::ios::in);
    loadFromStream(file);
}


void MvxPlayer::play(float instrumentalVolume, float pianoVolume) {
    vxPlayer.setVolume(pianoVolume);
    instrumentalPlayer.setVolume(instrumentalVolume);
    vxPlayer.play(vxAudioData.data(), vxAudioData.size(), 0);
    instrumentalPlayer.play(audioData.data(), audioData.size(), 0);
}

MvxPlayer::MvxPlayer() {

}

void MvxPlayer::pause() {
    instrumentalPlayer.pause();
    vxPlayer.pause();
}

void MvxPlayer::resume() {

}

void MvxPlayer::seek(double value) {
    instrumentalPlayer.seek(value);
    vxPlayer.seek(value);
}

void MvxPlayer::setInstrumentalVolume(float instrumentalVolume) {
    instrumentalPlayer.setVolume(instrumentalVolume);
}

void MvxPlayer::setPianoVolume(float pianoVolume) {
    vxPlayer.setVolume(pianoVolume);
}
