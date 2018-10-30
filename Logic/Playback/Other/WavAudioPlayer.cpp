//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <algorithm>
#include <cstring>

#include "WavAudioPlayer.h"
#include "WAVFile.h"

const int FRAMES_PER_BUFFER = 256;

const char *WavAudioPlayer::provideAudioBuffer() {
    return audioData.data() + WAVFile::DATA_POSITION;
}

void WavAudioPlayer::setAudioData(std::string &&audioData) {
    this->audioData = std::move(audioData);
    setPlaybackData(WAVFile::parseWavHeader(this->audioData), FRAMES_PER_BUFFER);
}

int WavAudioPlayer::getAudioDataSizeInBytes() {
    return (int)audioData.size() - WAVFile::DATA_POSITION;
}

