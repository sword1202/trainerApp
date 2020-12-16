#include "AudioOutputWriter.h"
#include "config.h"
#include <AudioToolbox/AudioToolbox.h>
#include "AudioToolboxQueue.h"

class AudioToolboxOutputWriter : public AudioOutputWriter {
    AudioToolboxQueue queue;
    int sampleSize;

    static void audioQueueCallback(void* inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer) {
        AudioToolboxOutputWriter* self = static_cast<AudioToolboxOutputWriter *>(inUserData);
        self->callback(inBuffer->mAudioData, inBuffer->mAudioDataByteSize / self->sampleSize);
    }
public:
    AudioToolboxOutputWriter(const AudioStreamDescription& data) {
        sampleSize = data.getSampleBytesCount();
        queue.initAsOutput(data, audioQueueCallback, this);
    }

    void start() override {
        queue.start();
    }

    void stop() override {
        queue.pause();
    }
};

AudioOutputWriter* AudioOutputWriter::create(const AudioStreamDescription& data) {
    return new AudioToolboxOutputWriter(data);
}