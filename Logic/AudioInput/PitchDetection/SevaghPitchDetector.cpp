//
// Created by Semyon Tikhonenko on 12/16/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#include "SevaghPitchDetector.h"
#include "AudioUtils.h"

void SevaghPitchDetector::init(int maxBufferSize, int sampleRate) {
    yin = new pitch_alloc::Yin<float>(maxBufferSize);
    this->sampleRate = sampleRate;
    tempFloatBuffer.resize(static_cast<size_t>(maxBufferSize));
}

float SevaghPitchDetector::getFrequencyFromBuffer(const int16_t *buffer) {
    AudioUtils::Int16SamplesIntoFloatSamples(buffer, static_cast<int>(tempFloatBuffer.size()), tempFloatBuffer.data());
    return yin->pitch(tempFloatBuffer, sampleRate);
}
