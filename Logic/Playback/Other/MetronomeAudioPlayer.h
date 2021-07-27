//
// Created by Semyon Tikhonenko on 7/23/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_METRONOMEAUDIOPLAYER_H
#define VOCALTRAINER_METRONOMEAUDIOPLAYER_H

#include "WavAudioPlayer.h"
#include "StlContainerAudioDataBuffer.h"
#include <string>

class MetronomeAudioPlayer : public BaseRawPcmAudioDataPlayer {
    double beatsPerMinute = 0;
    std::string metronomeAudioData;
    int totalVirtualBufferSize;
    std::unique_ptr<StdStringAudioDataBuffer> audioData;
public:
    MetronomeAudioPlayer();

    void setMetronomeAudioData(std::string&& metronomeAudioData);

    double getBeatsPerMinute() const;
    void setAudioDataInfo(double beatsPerMinute, double totalDurationInSeconds);

protected:
    int getAudioDataSizeInBytes() const override;
    int readAudioData(void *into, int offset, int numberOfBytes) override;
};


#endif //VOCALTRAINER_METRONOMEAUDIOPLAYER_H
