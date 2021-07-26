//
// Created by Semyon Tikhonenko on 12/6/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#include "AudioToolboxQueue.h"
#include "AudioToolboxUtils.h"
#include <iostream>

using namespace std;

void AudioToolboxQueue::HandleInputBuffer(void *userData,
        AudioQueueRef audioQueueRef,
        AudioQueueBufferRef inBuffer,
        const AudioTimeStamp *inStartTime,
        UInt32 inNumPackets,
        const AudioStreamPacketDescription *inPacketDesc) {
    AudioToolboxQueue* self = static_cast<AudioToolboxQueue *>(userData);
    self->inputCallback(self->userData, audioQueueRef, inBuffer, inStartTime, inNumPackets, inPacketDesc);
    auto status = AudioQueueEnqueueBuffer(self->queue, inBuffer, 0, NULL);
    AudioToolboxUtils::throwExceptionIfError(status);
}

void AudioToolboxQueue::HandleOutputBuffer(void *inUserData, AudioQueueRef queue, AudioQueueBufferRef inBuffer) {
    AudioToolboxQueue* self = static_cast<AudioToolboxQueue *>(inUserData);
    self->outputCallback(self->userData, queue, inBuffer);
    auto status = AudioQueueEnqueueBuffer(self->queue, inBuffer, 0, NULL);
    AudioToolboxUtils::throwExceptionIfError(status);
}

void AudioToolboxQueue::initAsInput(const AudioStreamDescription &description, AudioQueueInputCallback callback, void* userData) {
    assert(!queue && "queue has been already initialized");
    inputCallback = callback;
    this->userData = userData;
    AudioStreamBasicDescription audioToolboxFormat;
    AudioToolboxUtils::createFormat(description, &audioToolboxFormat);

    auto status = AudioQueueNewInput(&audioToolboxFormat,
            HandleInputBuffer,
            this,
            NULL, // Use internal thread
            kCFRunLoopCommonModes,
            0, // Reserved, must be 0
            &queue);
    AudioToolboxUtils::throwExceptionIfError(status);
    allocateBuffers(description.getCallbackBufferSizeInBytes(), nullptr);
}

void AudioToolboxQueue::initAsOutput(const AudioStreamDescription &description, AudioQueueOutputCallback callback, void* userData) {
    assert(!queue && "queue has been already initialized");
    outputCallback = callback;
    this->userData = userData;
    AudioStreamBasicDescription audioToolboxFormat;
    AudioToolboxUtils::createFormat(description, &audioToolboxFormat);

    auto status = AudioQueueNewOutput(&audioToolboxFormat,
            HandleOutputBuffer,
            this,
            NULL, // Use internal thread
            kCFRunLoopCommonModes,
            0, // Reserved, must be 0
            &queue);
    AudioToolboxUtils::throwExceptionIfError(status);
    AudioStreamPacketDescription packetDescription;
    packetDescription.mDataByteSize = static_cast<UInt32>(description.getCallbackBufferSizeInBytes());
    packetDescription.mStartOffset = 0;
    packetDescription.mVariableFramesInPacket = 0;
    allocateBuffers(description.getCallbackBufferSizeInBytes(), NULL);
}

void AudioToolboxQueue::start() {
    assert(queue);
    auto status = AudioQueueStart(queue, NULL);
    AudioToolboxUtils::throwExceptionIfError(status);
}

void AudioToolboxQueue::pause() {
    assert(queue);
    auto status = AudioQueuePause(queue);
    AudioToolboxUtils::throwExceptionIfError(status);
}

AudioToolboxQueue::~AudioToolboxQueue() {
    if (queue) {
        OSStatus status = AudioQueueDispose(queue, false);
        AudioToolboxUtils::throwExceptionIfError(status);
        queue = NULL;
    }
}

void AudioToolboxQueue::allocateBuffers(int bufferSizeInBytes, const AudioStreamPacketDescription* audioStreamPacketDescription) {
    for (int i = 0; i < kNumberBuffers; ++i) {
        auto status = AudioQueueAllocateBuffer(queue,
                static_cast<UInt32>(bufferSizeInBytes),
                &buffers[i]);
        AudioToolboxUtils::throwExceptionIfError(status);
        buffers[i]->mAudioDataByteSize = buffers[i]->mAudioDataBytesCapacity;
        memset(buffers[i]->mAudioData, 0, buffers[i]->mAudioDataByteSize);

        status = AudioQueueEnqueueBuffer(queue,
                buffers[i],
                audioStreamPacketDescription == nullptr ? 0 : 1,
                audioStreamPacketDescription);
        AudioToolboxUtils::throwExceptionIfError(status);
    }
}
