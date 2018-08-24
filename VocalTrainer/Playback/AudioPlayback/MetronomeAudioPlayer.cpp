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

    std::string audioData;
    audioData.reserve((size_t)totalBufferSize);
    audioData.assign(beatBufferSize - metronomeBufferSize / 2, '\0');

    while (audioData.size() < totalBufferSize) {
        audioData.append(metronomeAudioData.data() + WAVFile::DATA_POSITION, (size_t)metronomeBufferSize);
        audioData.append(beatBufferSize - metronomeBufferSize, '\0');
    }

    while (audioData.size() != totalBufferSize) {
        audioData.pop_back();
    }

    setAudioData(std::move(audioData));
}

BaseWavAudioPlayer::WavSetupData MetronomeAudioPlayer::provideWavSetupData() {
    assert(!metronomeAudioData.empty());
    return WavSetupData(metronomeAudioData.data(), metronomeAudioData.size());
}

double MetronomeAudioPlayer::calculateTotalDurationInSeconds(int size, int bytesPerChannel,
        const AudioPlayer::PlaybackData &playbackData) {
    return 0;
}
