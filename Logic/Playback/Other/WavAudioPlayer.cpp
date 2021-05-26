//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <algorithm>
#include <cstring>

#include "WavAudioPlayer.h"
#include "WAVFile.h"

const int FRAMES_PER_BUFFER = 256;

AudioDataBufferConstPtr WavAudioPlayer::provideAudioBuffer() {
    return audioData;
}

int WavAudioPlayer::getAudioDataSizeInBytes() {
    return audioData->getNumberOfBytes() - WAVFile::DATA_POSITION;
}

WavAudioPlayer::WavAudioPlayer() {
    setPlayerName("WavAudioPlayer");
    initSoundTouch();
}

void WavAudioPlayer::setAudioData(AudioDataBufferConstPtr audioData) {
    this->audioData = audioData;
    setPlaybackData(audioData->parseWavHeader(), FRAMES_PER_BUFFER);
}

