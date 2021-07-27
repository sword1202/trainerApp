//
// Created by Semyon Tikhonenko on 7/23/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <cmath>
#include "MetronomeAudioPlayer.h"
#include "StringUtils.h"
#include "config.h"
#include <iostream>

using namespace CppUtils;

using std::cout;
using std::endl;

CPP_UTILS_DLLHIDE class Buffer : public StdStringAudioDataBuffer {
public:
    Buffer() {
    }

    int read(void *into, int offset, int numberOfBytes) const override {
        offset %= getNumberOfBytes();
        return StdStringAudioDataBuffer::read(into, offset, numberOfBytes);
    }
};

void MetronomeAudioPlayer::setMetronomeAudioData(std::string&& metronomeAudioData) {
    this->audioData.reset(new Buffer());
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

    std::string& data = audioData->getData();
    data.clear();
    data.reserve((size_t)totalBufferSize);
    data.append(beatBufferSize - metronomeBufferSize / 2, '\0');
    data.append(metronomeAudioData.data() + WAVFile::DATA_POSITION, (size_t)metronomeBufferSize);

    while (data.size() % getPlaybackData().getCallbackBufferSizeInBytes() != 0) {
        data.push_back('\0');
    }
}

int MetronomeAudioPlayer::getAudioDataSizeInBytes() const {
    return totalVirtualBufferSize;
}

MetronomeAudioPlayer::MetronomeAudioPlayer() {
    setPlayerName("MetronomeAudioPlayer");
}

int MetronomeAudioPlayer::readAudioData(void *into, int offset, int numberOfBytes) {
    return audioData->read(into, offset, numberOfBytes);
}
