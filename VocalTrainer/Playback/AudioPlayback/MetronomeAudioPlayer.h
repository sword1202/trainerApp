//
// Created by Semyon Tikhonenko on 7/23/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_METRONOMEAUDIOPLAYER_H
#define VOCALTRAINER_METRONOMEAUDIOPLAYER_H

#include "WavAudioPlayer.h"
#include <string>

class MetronomeAudioPlayer : public WavAudioPlayer {
    double beatsPerMinute = 0;
    std::string metronomeAudioData;
public:
    void setMetronomeAudioData(std::string&& metronomeAudioData);

    double getBeatsPerMinute() const;
    void setAudioDataInfo(double beatsPerMinute, double totalDurationInSeconds);

protected:
    virtual double calculateTotalDurationInSeconds(int size, int bytesPerChannel, const PlaybackData &playbackData) override;
    virtual WavSetupData provideWavSetupData() override;
};


#endif //VOCALTRAINER_METRONOMEAUDIOPLAYER_H
