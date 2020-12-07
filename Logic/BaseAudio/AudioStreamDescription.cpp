//
// Created by Semyon Tikhonenko on 12/6/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#include "AudioStreamDescription.h"

AudioStreamDescription::AudioStreamDescription(const WavConfig &wavConfig, int samplesPerBuffer)
: WavConfig(wavConfig), samplesPerBuffer(samplesPerBuffer) {
}

int AudioStreamDescription::getBytesPerChannel() const {
    assert(bitsPerChannel % 8 == 0);
    return bitsPerChannel / 8;
}

int AudioStreamDescription::getSampleBytesCount() const {
    return getBytesPerChannel() * numberOfChannels;
}

int AudioStreamDescription::getCallbackBufferSizeInBytes() const {
    return samplesPerBuffer * getSampleBytesCount();
}
