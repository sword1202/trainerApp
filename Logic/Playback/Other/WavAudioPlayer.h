//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_WAVAUDIOPLAYER_H
#define VOCALTRAINER_WAVAUDIOPLAYER_H

#include "BaseRawPcmAudioDataPlayer.h"
#include "AudioDataBuffer.h"
#include <string>

class WavAudioPlayer : public BaseRawPcmAudioDataPlayer {
    AudioDataBufferConstPtr audioData = nullptr;
protected:
    AudioDataBufferConstPtr provideAudioBuffer() override;
    int getAudioDataSizeInBytes() override;
public:
    WavAudioPlayer();
public:
    virtual void setAudioData(AudioDataBufferConstPtr audioData);
};


#endif //VOCALTRAINER_WAVAUDIOPLAYER_H
