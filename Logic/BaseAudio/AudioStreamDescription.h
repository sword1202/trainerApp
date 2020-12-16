//
// Created by Semyon Tikhonenko on 12/6/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOSTREAMDESCRIPTION_H
#define VOCALTRAINER_AUDIOSTREAMDESCRIPTION_H

#include "WAVFile.h"

struct AudioStreamDescription : WavConfig {
    int samplesPerBuffer = -1;

    AudioStreamDescription() = default;
    AudioStreamDescription(const WavConfig &wavConfig, int samplesPerBuffer);

    int getBytesPerChannel() const;
    int getSampleBytesCount() const;
    int getCallbackBufferSizeInBytes() const;
};


#endif //VOCALTRAINER_AUDIOSTREAMDESCRIPTION_H
