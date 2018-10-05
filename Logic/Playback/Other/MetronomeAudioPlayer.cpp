//
// Created by Semyon Tikhonenko on 7/23/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <cmath>
#include "MetronomeAudioPlayer.h"
#include "WAVFile.h"
#include <iostream>
#include <cassert>


using std::cout;
using std::endl;


void MetronomeAudioPlayer::setMetronomeAudioData(std::string&& metronomeAudioData) {
    this->metronomeAudioData = std::move(metronomeAudioData);
    setPlaybackData(WAVFile::parseWavHeader(this->metronomeAudioData));
}

double MetronomeAudioPlayer::getBeatsPerMinute() const {
    return beatsPerMinute;
}

void MetronomeAudioPlayer::setAudioDataInfo(double beatsPerMinute, double totalDurationInSeconds) {
    this->beatsPerMinute = beatsPerMinute;
    setTotalDurationInSeconds(totalDurationInSeconds);

    int sampleSize = getSampleSize();
    double beatDuration = 60.0 / beatsPerMinute;
    int beatBufferSize = secondsToSamplesCount(beatDuration) * sampleSize;
    int metronomeBufferSize = (int)metronomeAudioData.size() - WAVFile::DATA_POSITION;

    assert(beatBufferSize - metronomeBufferSize >= 0);

    int totalBufferSize = secondsToSamplesCount(totalDurationInSeconds) * sampleSize;

    audioData.clear();
    audioData.reserve((size_t)totalBufferSize);
    audioData.append(beatBufferSize - metronomeBufferSize / 2, '\0');

    while (audioData.size() < totalBufferSize) {
        audioData.append(metronomeAudioData.data() + WAVFile::DATA_POSITION, (size_t)metronomeBufferSize);
        audioData.append(beatBufferSize - metronomeBufferSize, '\0');
    }

    while (audioData.size() != totalBufferSize) {
        audioData.pop_back();
    }
}

int MetronomeAudioPlayer::getAudioDataSizeInBytes() {
    return audioData.size();
}

const char *MetronomeAudioPlayer::provideAudioBuffer() {
    return audioData.data();
}
