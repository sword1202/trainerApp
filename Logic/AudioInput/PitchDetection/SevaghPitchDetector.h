//
// Created by Semyon Tikhonenko on 12/16/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_SEVAGHPITCHDETECTOR_H
#define VOCALTRAINER_SEVAGHPITCHDETECTOR_H

#include "PitchDetector.h"
#include "pitch_detection.h"

class SevaghPitchDetector : public PitchDetector {
    pitch_alloc::Yin<float>* yin;
    int sampleRate;
    std::vector<float> tempFloatBuffer;
public:
    void init(int maxBufferSize, int sampleRate) override;
    float getFrequencyFromBuffer(const int16_t *buffer) override;
};


#endif //VOCALTRAINER_SEVAGHPITCHDETECTOR_H
