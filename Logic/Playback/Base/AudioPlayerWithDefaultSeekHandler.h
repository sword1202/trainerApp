//
// Created by Semyon Tikhonenko on 7/23/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOPLAYERWITHDEFAULTSEEKHANDLER_H
#define VOCALTRAINER_AUDIOPLAYERWITHDEFAULTSEEKHANDLER_H


#include "AudioPlayer.h"

class AudioPlayerWithDefaultSeekHandler : public AudioPlayer {
    int bufferSeek = 0;
    mutable std::mutex bufferSeekMutex;
protected:
    int getBufferSeek() const override;
    void setBufferSeek(int bufferSeek) override;
    // handle a case, when seek is set from outside while audioDecoder is reading the data
    void moveBufferSeekIfNotChangedBefore(int moveBy, int seekBefore);
};


#endif //VOCALTRAINER_AUDIOPLAYERWITHDEFAULTSEEKHANDLER_H