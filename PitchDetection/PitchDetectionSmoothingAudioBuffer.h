//
// Created by Semyon Tikhonenko on 1/14/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PITCHDETECTIONAUDIOTEMPORARYBUFFER_H
#define VOCALTRAINER_PITCHDETECTIONAUDIOTEMPORARYBUFFER_H


#include <cstdint>
#include "CppUtils/stlassert.h"
#include "CppUtils/CircularBuffer.h"
#include <vector>

class PitchDetectionSmoothingAudioBuffer {
public:
    PitchDetectionSmoothingAudioBuffer(size_t smoothLevel, size_t sampleSize);
    const int16_t* getRunPitchDetectionBufferIfReady(const int16_t *sampleData, size_t dataSize);

    size_t getCurrentBufferSize() const;

private:
    std::vector<int16_t> buffer;
    CppUtils::CircularBuffer<size_t> piecesSizes;
    size_t currentBufferSize = 0;
    size_t smoothLevel;
    size_t sampleSize;
};


#endif //VOCALTRAINER_PITCHDETECTIONAUDIOTEMPORARYBUFFER_H
