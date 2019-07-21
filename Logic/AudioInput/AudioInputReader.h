//
// Created by Semyon Tikhonenko on 8/7/16.
//

#ifndef PITCHDETECTION_AUDOINPUTREADER_H
#define PITCHDETECTION_AUDOINPUTREADER_H


#include <cstdint>
#include "ListenersSet.h"
#include "WAVFile.h"

class AudioInputReader {
public:
    typedef std::function<void(const int16_t*, int)> Callback;

    CppUtils::SynchronizedListenersSet<const int16_t*, int> callbacks;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool isRunning() = 0;
    virtual int getSampleRate() const = 0;
    virtual int getSampleSizeInBytes() const = 0;
    virtual int getMaximumBufferSize() const = 0;
    virtual void setSensitivity(float value) = 0;
    virtual float getSensitivity() const = 0;
    virtual int getNumberOfChannels() const = 0;
    virtual WavConfig generateWavConfig() const = 0;

    virtual const char* getDeviceName() const = 0;
    virtual void setDeviceName(const char* deviceName) = 0;

    virtual ~AudioInputReader() = default;
};

class AudioInputReaderWithOutput : public AudioInputReader {
public:
    virtual void setOutputVolume(float value) = 0;
    virtual float getOutputVolume() const = 0;
};

#endif //PITCHDETECTION_AUDOINPUTREADER_H
