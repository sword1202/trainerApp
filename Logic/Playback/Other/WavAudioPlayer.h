//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_WAVAUDIOPLAYER_H
#define VOCALTRAINER_WAVAUDIOPLAYER_H

#include "BaseRawPcmAudioDataPlayer.h"
#include <string>

class WavAudioPlayer : public BaseRawPcmAudioDataPlayer {
    std::string audioData;
protected:
    const char *provideAudioBuffer() override;
    int getAudioDataSizeInBytes() override;
public:
    virtual void setAudioData(std::string&& audioData);
};


#endif //VOCALTRAINER_WAVAUDIOPLAYER_H
