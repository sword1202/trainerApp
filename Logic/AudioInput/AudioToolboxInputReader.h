//
// Created by Semyon Tikhonenko on 12/5/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOTOOLBOXINPUTREADER_H
#define VOCALTRAINER_AUDIOTOOLBOXINPUTREADER_H

#include "AudioInputReader.h"
#include <AudioToolbox/AudioToolbox.h>
#include "AudioStreamDescription.h"
#include "AudioToolboxQueue.h"

class AudioToolboxInputReader : public AudioInputReaderWithOutput {
    volatile bool running = false;
    AudioStreamDescription format;
    AudioToolboxQueue queue;

    static void HandleInputBuffer(void *userData,
            AudioQueueRef audioQueueRef,
            AudioQueueBufferRef inBuffer,
            const AudioTimeStamp *inStartTime,
            UInt32 inNumPackets,
            const AudioStreamPacketDescription *inPacketDesc);
public:
    AudioToolboxInputReader(int maximumBufferSize);

    void start() override;
    void stop() override;
    bool isRunning() override;
    int getSampleRate() const override;
    int getSampleSizeInBytes() const override;
    int getMaximumBufferSize() const override;
    int getNumberOfChannels() const override;
    WavConfig generateWavConfig() const override;
    const char *getDeviceName() const override;
    void setDeviceName(const char *deviceName) override;

    void setOutputVolume(float value) override;
    float getOutputVolume() const override;
};


#endif //VOCALTRAINER_AUDIOTOOLBOXINPUTREADER_H
