//
// Created by Semyon Tikhonenko on 1/15/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MvxPlayer.h"
#include "Strings.h"

using namespace CppUtils;

void MvxPlayer::load(std::istream &audioFile, std::istream &vxFile) {
    audioData = Strings::StreamToString(audioFile);
    vxAudioData = VxFile(vxFile).generateWavAudioData();
}

void MvxPlayer::play() {
    vxPlayer.play(vxAudioData.data(), vxAudioData.size(), 0);
    player.play(audioData.data(), audioData.size(), 0);
}

MvxPlayer::MvxPlayer() {
}

void MvxPlayer::pause() {
    player.pause();
    vxPlayer.pause();
}

void MvxPlayer::resume() {

}

void MvxPlayer::seek(double value) {
    player.seek(value);
    vxPlayer.seek(value);
}
