//
// Created by Semyon Tikhonenko on 8/7/16.
//

#ifndef PITCHDETECTION_AUDOINPUTREADER_H
#define PITCHDETECTION_AUDOINPUTREADER_H


#include <cstdint>
#include <functional>
#include "FunctionsList.h"

static const int kNumberBuffers = 3;

class AudioInputReader {
public:
    typedef std::function<void(const int16_t*, int)> Callback;

    CppUtils::FunctionsList<const int16_t*, int> callbacks;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual int getSampleRate() const = 0;
    virtual int getMaximumBufferSize() const = 0;
    virtual void setInputVolume(float value) = 0;
    virtual float getInputVolume() const = 0;

    virtual ~AudioInputReader() = default;
};

AudioInputReader* CreateDefaultAudioInputReader(int maximumBufferSize);

#endif //PITCHDETECTION_AUDOINPUTREADER_H
