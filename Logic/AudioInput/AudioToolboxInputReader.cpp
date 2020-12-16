//
// Created by Semyon Tikhonenko on 12/5/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#include "AudioToolboxInputReader.h"
#include "NotImplementedAssert.h"
#include <AudioToolbox/AudioToolbox.h>
#include "AudioStreamDescription.h"
#include "AudioToolboxUtils.h"

void AudioToolboxInputReader::HandleInputBuffer(void *userData,
        AudioQueueRef audioQueueRef,
        AudioQueueBufferRef inBuffer,
        const AudioTimeStamp *inStartTime,
        UInt32 inNumPackets,
        const AudioStreamPacketDescription *inPacketDesc)
{
    AudioToolboxInputReader* self = static_cast<AudioToolboxInputReader *>(userData);
    self->callbacks.executeAll((int16_t*)inBuffer->mAudioData, inBuffer->mAudioDataByteSize / sizeof(int16_t));
}

AudioToolboxInputReader::AudioToolboxInputReader(int maximumBufferSize) {
    assert(maximumBufferSize > 0);
    format.sampleRate = 44100;
    format.numberOfChannels = 1;
    format.samplesPerBuffer = maximumBufferSize;
    format.bitsPerChannel = 16;
    queue.initAsInput(format, HandleInputBuffer, this);
}

void AudioToolboxInputReader::start() {
    running = true;
    queue.start();
}

void AudioToolboxInputReader::stop() {
    running = false;
    queue.pause();
}

bool AudioToolboxInputReader::isRunning() {
    return running;
}

int AudioToolboxInputReader::getSampleRate() const {
    return format.sampleRate;
}

int AudioToolboxInputReader::getSampleSizeInBytes() const {
    return format.getSampleBytesCount();
}

int AudioToolboxInputReader::getMaximumBufferSize() const {
    return format.getCallbackBufferSizeInBytes() / sizeof(int16_t);
}

int AudioToolboxInputReader::getNumberOfChannels() const {
    return format.numberOfChannels;
}

WavConfig AudioToolboxInputReader::generateWavConfig() const {
    return format;
}

const char *AudioToolboxInputReader::getDeviceName() const {
    return "";
}

void AudioToolboxInputReader::setDeviceName(const char *deviceName) {
    NOT_IMPLEMENTED_ASSERT;
}

void AudioToolboxInputReader::setOutputVolume(float value) {
    NOT_IMPLEMENTED_ASSERT;
}

float AudioToolboxInputReader::getOutputVolume() const {
    return 0;
}
