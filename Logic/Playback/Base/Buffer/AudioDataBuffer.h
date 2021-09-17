//
// Created by Semyon Tykhonenko on 8/24/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIODATABUFFER_H
#define VOCALTRAINER_AUDIODATABUFFER_H

#include <string>
#include "WAVFile.h"
#include "DestructorQueue.h"
#include <memory>
#include "Serializers.h"

class AudioDataBuffer {
public:
    // Reads data from the buffer, returns number of bytes read(Usually == numberOfBytes, unless at the end of the buffer)
    virtual int read(void* into, int offset, int numberOfBytes) const = 0;
    virtual int getNumberOfBytes() const = 0;
    virtual WavConfig parseWavHeader() const;
    virtual std::string toBinaryString() const;
    virtual ~AudioDataBuffer() = default;
};

using AudioDataBufferPtr = std::shared_ptr<AudioDataBuffer>;
using AudioDataBufferConstPtr = std::shared_ptr<const AudioDataBuffer>;

#endif //VOCALTRAINER_AUDIODATABUFFER_H
