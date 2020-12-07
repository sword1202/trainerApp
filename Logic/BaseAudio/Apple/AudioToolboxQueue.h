//
// Created by Semyon Tikhonenko on 12/6/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOTOOLBOXQUEUE_H
#define VOCALTRAINER_AUDIOTOOLBOXQUEUE_H

#include <AudioToolbox/AudioToolbox.h>
#include "AudioStreamDescription.h"
#include <functional>

class AudioToolboxQueue {
    static const int kNumberBuffers = 3;

    void* userData;
    AudioQueueRef queue = NULL;
    AudioQueueInputCallback inputCallback;
    AudioQueueOutputCallback outputCallback;
    AudioQueueBufferRef buffers[kNumberBuffers];

    static void HandleInputBuffer(void *userData,
            AudioQueueRef audioQueueRef,
            AudioQueueBufferRef inBuffer,
            const AudioTimeStamp *inStartTime,
            UInt32 inNumPackets,
            const AudioStreamPacketDescription *inPacketDesc);

    static void HandleOutputBuffer(void* inUserData, AudioQueueRef queue, AudioQueueBufferRef inBuffer);

    void allocateBuffers(int bufferSizeInBytes, const AudioStreamPacketDescription* audioStreamPacketDescription);
public:
    void initAsInput(const AudioStreamDescription &description, AudioQueueInputCallback callback, void* userData);
    void initAsOutput(const AudioStreamDescription &description, AudioQueueOutputCallback callback, void* userData);
    void start();
    void pause();

    ~AudioToolboxQueue();
};


#endif //VOCALTRAINER_AUDIOTOOLBOXQUEUE_H
