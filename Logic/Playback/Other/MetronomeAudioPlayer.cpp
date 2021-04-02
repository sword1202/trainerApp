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


void MetronomeAudioPlayer::setMetronomeAudioData(AudioData&& metronomeAudioData) {
    this->metronomeAudioData = std::move(metronomeAudioData);
    setPlaybackData(WAVFile::parseWavHeader(this->metronomeAudioData));
}

double MetronomeAudioPlayer::getBeatsPerMinute() const {
    return beatsPerMinute;
}

void MetronomeAudioPlayer::setAudioDataInfo(double beatsPerMinute, double totalDurationInSeconds) {
    assert(beatsPerMinute > 0);

    this->beatsPerMinute = beatsPerMinute;
    setTotalDurationInSeconds(totalDurationInSeconds);

    int sampleSize = getSampleSize();
    double beatDuration = 60.0 / beatsPerMinute;
    int beatBufferSize = secondsSeekToBufferSeek(beatDuration) * sampleSize;
    int metronomeBufferSize = (int)metronomeAudioData.size() - WAVFile::DATA_POSITION;

    assert(beatBufferSize - metronomeBufferSize >= 0);

    this->totalVirtualBufferSize = secondsSeekToBufferSeek(totalDurationInSeconds) * sampleSize;
    int totalBufferSize = beatBufferSize + metronomeBufferSize / 2;

    audioData.clear();
    audioData.reserve((size_t)totalBufferSize);
    audioData.append(beatBufferSize - metronomeBufferSize / 2, '\0');
    audioData.append(metronomeAudioData.data() + WAVFile::DATA_POSITION, (size_t)metronomeBufferSize);

    while (audioData.size() % getPlaybackData().getCallbackBufferSizeInBytes() != 0) {
        audioData.push_back('\0');
    }
}

int MetronomeAudioPlayer::getAudioDataSizeInBytes() {
    return totalVirtualBufferSize;
}

MetronomeAudioPlayer::MetronomeAudioPlayer() {
    setPlayerName("MetronomeAudioPlayer");
}

const char *MetronomeAudioPlayer::provideAudioBuffer(int offset) {
    offset %= audioData.size();
    assert(offset < audioData.size());
    return audioData.data() + offset;
}
