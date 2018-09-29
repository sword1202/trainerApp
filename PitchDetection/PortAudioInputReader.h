//
// Created by Semyon Tikhonenko on 9/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PORTAUDIOINPUTREADER_H
#define VOCALTRAINER_PORTAUDIOINPUTREADER_H

#include "AudioInputReader.h"
#include <portmixer.h>

class PortAudioInputReader : public AudioInputReaderWithOutput {
    static int portAudioCallback(const void *inputBuffer,
            void *outputBuffer,
            unsigned long framesPerBuffer,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void *userData);

    int maximumBufferSize;
    PaStream *stream;
    PxMixer *mixer;
    bool outputEnabled;
    std::atomic<float> outputVolume;
public:
    PortAudioInputReader(int maximumBufferSize, bool outputEnabled, int deviceIndex = -1);
    ~PortAudioInputReader();

    void start() override;
    void stop() override;

    int getSampleRate() const override;
    int getMaximumBufferSize() const override;

    void setInputVolume(float value) override;
    float getInputVolume() const override;
    void setOutputVolume(float value) override;
    float getOutputVolume() const override;

    bool isOutputEnabled() const;
};


#endif //VOCALTRAINER_PORTAUDIOINPUTREADER_H
