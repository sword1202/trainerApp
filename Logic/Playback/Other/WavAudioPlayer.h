//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_WAVAUDIOPLAYER_H
#define VOCALTRAINER_WAVAUDIOPLAYER_H

#include "BaseRawPcmAudioDataPlayer.h"
#include "AudioData.h"
#include <string>

class WavAudioPlayer : public BaseRawPcmAudioDataPlayer {
    const std::string* audioData = nullptr;
protected:
    const char *provideAudioBuffer(int offset) override;
    int getAudioDataSizeInBytes() override;
public:
    WavAudioPlayer();

    virtual void setAudioData(const AudioData *audioData);
};


#endif //VOCALTRAINER_WAVAUDIOPLAYER_H
