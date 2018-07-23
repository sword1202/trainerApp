//
// Created by Semyon Tikhonenko on 7/23/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_METRONOMEAUDIOPLAYER_H
#define VOCALTRAINER_METRONOMEAUDIOPLAYER_H

#include "BaseWavAudioPlayer.h"
#include <string>

class MetronomeAudioPlayer : public BaseWavAudioPlayer {
    double beatsPerMinute = 0;
    std::string metronomeAudioData;
    double totalDurationInSeconds;
    int totalSamplesCount;
public:
    void setMetronomeAudioData(std::string&& metronomeAudioData);

    double getBeatsPerMinute() const;
    void setAudioDataInfo(double beatsPerMinute, double totalDurationInSeconds);

protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    double calculateTotalDurationInSeconds(int size, int bytesPerChannel, const PlaybackData &playbackData) override;
    WavSetupData provideWavSetupData() override;
};


#endif //VOCALTRAINER_METRONOMEAUDIOPLAYER_H
