//
// Created by Semyon Tikhonenko on 7/23/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <cmath>
#include "MetronomeAudioPlayer.h"
#include "WAVFile.h"
#include <iostream>

using std::cout;
using std::endl;

int MetronomeAudioPlayer::readNextSamplesBatch(void *intoBuffer, int framesCount,
        const AudioPlayer::PlaybackData &playbackData) {
    assert(beatsPerMinute > 0);
    int sampleSize = getSampleSize();
    int seek = getBufferSeek();
    int result = std::min(framesCount, totalSamplesCount - seek);
    if (getVolume() <= 0.00001f) {
        memset(intoBuffer, 0, (size_t)result * sampleSize);
    } else {
        double beatDuration = 60.0 / beatsPerMinute;
        double timeSeek = getSeek();
        double mod = fmod(timeSeek, beatDuration);
        double halfMetronomeAudioDataDuration = samplesCountToSeconds(metronomeAudioData.size() / 2);
        double wavSeek = -1;
        double distanceToNextBeatBorder = beatDuration - mod - halfMetronomeAudioDataDuration;
        if (distanceToNextBeatBorder <= 0) {
            wavSeek = -distanceToNextBeatBorder;
        } else {
            double distanceToPrevBeatBorder = mod - halfMetronomeAudioDataDuration;
            if (distanceToPrevBeatBorder < 0) {
                wavSeek = halfMetronomeAudioDataDuration * 2 + distanceToPrevBeatBorder;
            }
        }

        if (wavSeek < 0) {
            memset(intoBuffer, 0, (size_t)result * sampleSize);
        } else {
            int wavBufferSeek = secondsToSamplesCount(wavSeek);
            if ((metronomeAudioData.size() - WAVFile::DATA_POSITION) / sampleSize - wavBufferSeek - result < 0) {
                memset(intoBuffer, 0, (size_t)result * sampleSize);
            } else {
                const char* begin = metronomeAudioData.data() + WAVFile::DATA_POSITION + wavBufferSeek * sampleSize;
                memcpy(intoBuffer, begin, (size_t)result * sampleSize);
            }
        }
    }

    moveBufferSeekIfNotChangedBefore(result, seek);
    return result;
}

void MetronomeAudioPlayer::setMetronomeAudioData(std::string&& metronomeAudioData) {
    this->metronomeAudioData = std::move(metronomeAudioData);
}

double MetronomeAudioPlayer::getBeatsPerMinute() const {
    return beatsPerMinute;
}

void MetronomeAudioPlayer::setAudioDataInfo(double beatsPerMinute, double totalDurationInSeconds) {
    this->beatsPerMinute = beatsPerMinute;
    this->totalDurationInSeconds = totalDurationInSeconds;
}

BaseWavAudioPlayer::WavSetupData MetronomeAudioPlayer::provideWavSetupData() {
    assert(!metronomeAudioData.empty());
    return WavSetupData(metronomeAudioData.data(), metronomeAudioData.size());
}

double MetronomeAudioPlayer::calculateTotalDurationInSeconds(int size, int bytesPerChannel,
        const AudioPlayer::PlaybackData &playbackData) {
    this->totalSamplesCount = secondsToSamplesCount(totalDurationInSeconds);
    return totalDurationInSeconds;
}
