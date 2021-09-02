//
// Created by Semyon Tikhonenko on 10/5/18.
//

#ifndef VOCALTRAINER_RAWPCMAUDIODATAPLAYER_H
#define VOCALTRAINER_RAWPCMAUDIODATAPLAYER_H

#include <string>
#include "WAVFile.h"
#include "AudioPlayerWithDefaultSeekHandler.h"
#include "AudioDataBuffer.h"

class BaseRawPcmAudioDataPlayer : public AudioPlayerWithDefaultSeekHandler {
public:
    void setPlaybackData(const WavConfig& wavConfig, int framesPerBuffer = 256);
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount,
            const PlaybackData &playbackData) override;
    virtual void providePlaybackData(PlaybackData *playbackData) override;
    virtual int getAudioDataSizeInBytes() const = 0;
    virtual int readAudioData(void* into, int offset, int numberOfBytes) = 0;
};


#endif //VOCALTRAINER_RAWPCMAUDIODATAPLAYER_H
