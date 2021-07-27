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
    int readAudioData(void *into, int offset, int numberOfBytes) override;
    int getAudioDataSizeInBytes() const override;
public:
    WavAudioPlayer();
public:
    virtual void setAudioData(AudioDataBufferConstPtr audioData);
    void reset() override;
};


#endif //VOCALTRAINER_WAVAUDIOPLAYER_H
