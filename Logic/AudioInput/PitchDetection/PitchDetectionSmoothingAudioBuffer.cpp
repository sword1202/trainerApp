//
// Created by Semyon Tikhonenko on 1/14/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#include <cstdlib>
#include <assert.h>
#include <cstring>
#include <iostream>
#include "PitchDetectionSmoothingAudioBuffer.h"

const int16_t* PitchDetectionSmoothingAudioBuffer::getRunPitchDetectionBufferIfReady(const int16_t *sampleData, size_t dataSize) {
    if (smoothLevel == 1) {
        currentBufferSize = dataSize;
        return sampleData;
    }

    if (piecesSizes.size() < piecesSizes.capacity()) {
        std::copy(sampleData, sampleData + dataSize, buffer.begin() + currentBufferSize);
        currentBufferSize += dataSize;
        piecesSizes.push_back(dataSize);
        if (piecesSizes.size() == piecesSizes.capacity()) {
            return buffer.data();
        }
    } else {
        size_t firstPieceBufferSize = piecesSizes.front();
        buffer.erase(buffer.begin(), buffer.begin() + firstPieceBufferSize);
        buffer.resize(smoothLevel * sampleSize);
        std::copy(sampleData, sampleData + dataSize, buffer.begin() + currentBufferSize - firstPieceBufferSize);
        currentBufferSize = currentBufferSize - firstPieceBufferSize + dataSize;
        piecesSizes.push_back(dataSize);
        return buffer.data();
    }

    return nullptr;
}

PitchDetectionSmoothingAudioBuffer::PitchDetectionSmoothingAudioBuffer(size_t smoothLevel, size_t sampleSize)
        : smoothLevel(smoothLevel), piecesSizes(smoothLevel), sampleSize(sampleSize)
{
    assert(smoothLevel >= 1);
    assert(sampleSize > 0);
    if (smoothLevel > 1) {
        size_t maxBufferSize = (size_t) (smoothLevel * sampleSize);
        buffer.resize(maxBufferSize);
    }
}

size_t PitchDetectionSmoothingAudioBuffer::getCurrentBufferSize() const {
    return currentBufferSize;
}
