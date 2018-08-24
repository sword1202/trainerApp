//
// Created by Semyon Tikhonenko on 8/7/16.
//

#include "AudioInputReader.h"
#include <AudioToolbox/AudioToolbox.h>

struct RecordingData {
    AudioInputReader::Callback callback;
    AudioQueueRef queue;
};

static void HandleInputBuffer(void *userData,
                  AudioQueueRef audioQueueRef,
                  AudioQueueBufferRef inBuffer,
                  const AudioTimeStamp *inStartTime,
                  UInt32 inNumPackets,
                  const AudioStreamPacketDescription *inPacketDesc)
{
    RecordingData* recordingData = (RecordingData*)userData;
    AudioInputReader::Callback& callback = recordingData->callback;
    if (callback) {
        callback((int16_t*)inBuffer->mAudioData, inBuffer->mAudioDataByteSize / sizeof(int16_t));
    }

    AudioQueueEnqueueBuffer(recordingData->queue,
                            inBuffer,
                            0,
                            NULL);
}

class OSXAudioInputReader : public AudioInputReader {
    static const int kSamplingRate = 44100;

    RecordingData recordingData;
    UInt32 bufferSizeInBytes;
    AudioQueueBufferRef buffers[kNumberBuffers];
public:
    OSXAudioInputReader(int maximumBufferSize) {
        assert(maximumBufferSize > 0);
        bufferSizeInBytes = (UInt32) maximumBufferSize * sizeof(int16_t);
        AudioStreamBasicDescription format;
        format.mSampleRate = kSamplingRate;
        format.mFormatID = kAudioFormatLinearPCM;
        format.mFramesPerPacket = 1; //For uncompressed audio, the value is 1. For variable bit-rate formats, the value is a larger fixed number, such as 1024 for AAC
        format.mChannelsPerFrame = 1;
        format.mBytesPerFrame = format.mChannelsPerFrame * 2;
        format.mBytesPerPacket = format.mFramesPerPacket * format.mBytesPerFrame;
        format.mBitsPerChannel = 16;
        format.mReserved = 0;
        format.mFormatFlags =  kAudioFormatFlagIsSignedInteger |
                                kAudioFormatFlagsNativeEndian |
                                kLinearPCMFormatFlagIsPacked;

        AudioQueueNewInput(&format,
                           HandleInputBuffer,
                           &recordingData,
                           NULL, // Use internal thread
                           kCFRunLoopCommonModes,
                           0, // Reserved, must be 0
                           &recordingData.queue);

        for (int i = 0; i < kNumberBuffers; ++i) {
            AudioQueueAllocateBuffer(recordingData.queue,
                                     bufferSizeInBytes,
                                     &buffers[i]);

            AudioQueueEnqueueBuffer(recordingData.queue,
                                    buffers[i],
                                    0,
                                    NULL);
        }
    }

    virtual void setCallback(Callback callback) override {
        recordingData.callback = callback;
    }

    virtual void start() override {
        AudioQueueStart(recordingData.queue, NULL);
    }

    virtual void stop() override {
        AudioQueuePause(recordingData.queue);
    }

    virtual ~OSXAudioInputReader() override {
        AudioQueueDispose(recordingData.queue, true);
    }

    virtual int getSampleRate() const override {
        return kSamplingRate;
    }

    virtual int getMaximumBufferSize() const override {
        return bufferSizeInBytes / sizeof(int16_t);
    }
};

AudioInputReader* CreateDefaultAudioInputReader(int maximumBufferSize) {
    return new OSXAudioInputReader(maximumBufferSize);
}

