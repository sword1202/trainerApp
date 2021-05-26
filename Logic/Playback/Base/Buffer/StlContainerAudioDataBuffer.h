//
// Created by Semyon Tikhonenko on 5/25/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_STLCONTAINERAUDIODATABUFFER_H
#define VOCALTRAINER_STLCONTAINERAUDIODATABUFFER_H

#include "AudioDataBuffer.h"
#include "DestructorQueue.h"
#include <string>

template <typename Container>
class StlContainerAudioDataBuffer : public AudioDataBuffer {
    Container data;
public:
    StlContainerAudioDataBuffer() {}
    StlContainerAudioDataBuffer(const Container& data) : data(data) {}
    StlContainerAudioDataBuffer(Container&& data) : data(std::move(data)) {}

    int getNumberOfBytes() const override {
        return data.size();
    }

    virtual int read(void *into, int offset, int numberOfBytes) const override {
        auto begin = data.begin() + offset;
        int size = getNumberOfBytes();
        assert(numberOfBytes >= 0);
        assert(offset >= 0 && offset <= size);
        int readBytesCount = std::min(numberOfBytes, size - offset);
        std::copy(begin, begin + readBytesCount, static_cast<char*>(into));
        return readBytesCount;
    }

    const Container& getData() const {
        return data;
    }

    Container& getData() {
        return data;
    }
};

class StdStringAudioDataBuffer : public StlContainerAudioDataBuffer<std::string> {
public:
    inline StdStringAudioDataBuffer(const std::string &data) : StlContainerAudioDataBuffer(data) {
    }

    inline StdStringAudioDataBuffer(std::string &&data) : StlContainerAudioDataBuffer(data) {
    }

    StdStringAudioDataBuffer() = default;

    inline std::string toBinaryString() const override {
        return getData();
    }
};

#endif //VOCALTRAINER_STLCONTAINERAUDIODATABUFFER_H
