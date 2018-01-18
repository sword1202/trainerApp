//
// Created by Semyon Tikhonenko on 1/15/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MvxPlayer.h"
#include "Strings.h"

using namespace CppUtils;

void MvxPlayer::load(std::istream &audioFile, std::istream &vxFile) {
    this->vxFile.load(vxFile);
    audioData = Strings::StreamToString(audioFile);
}

void MvxPlayer::play() {
    vxFile.play();
    player.play(audioData.data(), audioData.size(), 0);
}

MvxPlayer::MvxPlayer() {
}

void MvxPlayer::pause() {
    player.pause();
    vxFile.pause();
}

void MvxPlayer::resume() {

}

void MvxPlayer::seek(double value) {
    player.seek(value);
    vxFile.seek(value);
}
