//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_WAVAUDIOPLAYER_H
#define VOCALTRAINER_WAVAUDIOPLAYER_H

#include "BaseWavAudioPlayer.h"
#include <string>

class WavAudioPlayer : public BaseWavAudioPlayer {
    std::string audioData;
protected:
    virtual int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    virtual WavSetupData provideWavSetupData() override;

public:
    void setAudioData(std::string&& audioData);
};


#endif //VOCALTRAINER_WAVAUDIOPLAYER_H
