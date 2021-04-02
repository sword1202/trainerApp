//
// Created by Semyon Tikhonenko on 7/23/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_METRONOMEAUDIOPLAYER_H
#define VOCALTRAINER_METRONOMEAUDIOPLAYER_H

#include "WavAudioPlayer.h"
#include <string>

class MetronomeAudioPlayer : public BaseRawPcmAudioDataPlayer {
    double beatsPerMinute = 0;
    AudioData metronomeAudioData;
    AudioData audioData;
    int totalVirtualBufferSize;
public:
    MetronomeAudioPlayer();

    void setMetronomeAudioData(AudioData&& metronomeAudioData);

    double getBeatsPerMinute() const;
    void setAudioDataInfo(double beatsPerMinute, double totalDurationInSeconds);

protected:
    int getAudioDataSizeInBytes() override;

    const char *provideAudioBuffer(int offset) override;
};


#endif //VOCALTRAINER_METRONOMEAUDIOPLAYER_H
