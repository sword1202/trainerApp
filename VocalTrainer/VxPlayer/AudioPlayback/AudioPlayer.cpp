//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import <stdexcept>
#include "AudioPlayer.h"
#include <string>
#include "TimeUtils.h"

#import <iostream>

using namespace CppUtils;
using std::cout;

void AudioPlayer::play(const char *audioData, int size, double seek) {

}

bool AudioPlayer::isPlaying() {
    return false;
}

void AudioPlayer::stop() {

}

AudioPlayer::~AudioPlayer() {

}

void AudioPlayer::pause() {

}

void AudioPlayer::resume() {

}

void AudioPlayer::seek(double timeStamp) {

}

void AudioPlayer::setVolume(float volume) {

}

double AudioPlayer::getTrackDurationInSeconds() {
    return 0.0;
}
