//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <algorithm>
#include <cstring>

#include "WavAudioPlayer.h"
#include "WAVFile.h"

const int FRAMES_PER_BUFFER = 256;

int WavAudioPlayer::getAudioDataSizeInBytes() const {
    return audioData->getNumberOfBytes() - WAVFile::DATA_POSITION;
}

WavAudioPlayer::WavAudioPlayer() {
    setPlayerName("WavAudioPlayer");
    initSoundTouch();
}

void WavAudioPlayer::setAudioData(AudioDataBufferConstPtr audioData) {
    assert(!audioData && "Call reset to set audio data again");
    this->audioData = audioData;
    setPlaybackData(audioData->parseWavHeader(), FRAMES_PER_BUFFER);
}

void WavAudioPlayer::reset() {
    BaseAudioPlayer::reset();
    audioData = nullptr;
}

int WavAudioPlayer::readAudioData(void *into, int offset, int numberOfBytes) {
    return audioData->read(into, offset + WAVFile::DATA_POSITION, numberOfBytes);
}

