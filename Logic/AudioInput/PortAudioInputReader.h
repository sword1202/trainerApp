//
// Created by Semyon Tikhonenko on 9/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PORTAUDIOINPUTREADER_H
#define VOCALTRAINER_PORTAUDIOINPUTREADER_H

#include "AudioInputReader.h"
#include <portmixer.h>
#include <string>
#include "RealtimeStreamingAudioPlayer.h"

class PortAudioInputReader : public AudioInputReaderWithOutput {
    static int portAudioCallback(const void *inputBuffer,
            void *outputBuffer,
            unsigned long framesPerBuffer,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void *userData);

    int maximumBufferSize;
    PaStream *stream = nullptr;
    PxMixer *mixer = nullptr;
    bool outputEnabled;
    std::atomic<float> outputVolume;
    std::string deviceName;
    const PaDeviceInfo* device;

    void destroy();
public:
    PortAudioInputReader(int maximumBufferSize, bool outputEnabled, const char* deviceName = nullptr);
    ~PortAudioInputReader();

    void start() override;
    void stop() override;
    bool isRunning() override;

    int getSampleRate() const override;
    int getMaximumBufferSize() const override;

    void setInputVolume(float value) override;
    float getInputVolume() const override;
    void setOutputVolume(float value) override;
    float getOutputVolume() const override;

    bool isOutputEnabled() const;

    const char * getDeviceName() const override;
    void setDeviceName(const char* name) override;

    void init();

    int getSampleSizeInBytes() const override;
    int getNumberOfChannels() const override;
    WavConfig generateWavConfig() const override;
};


#endif //VOCALTRAINER_PORTAUDIOINPUTREADER_H
