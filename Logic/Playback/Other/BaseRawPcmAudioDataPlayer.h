//
// Created by Semyon Tikhonenko on 10/5/18.
//

#ifndef VOCALTRAINER_RAWPCMAUDIODATAPLAYER_H
#define VOCALTRAINER_RAWPCMAUDIODATAPLAYER_H

#include <string>
#include "WAVFile.h"
#include "AudioPlayerWithDefaultSeekHandler.h"

class BaseRawPcmAudioDataPlayer : public AudioPlayerWithDefaultSeekHandler {
public:
    void setPlaybackData(const WavConfig& wavConfig, int framesPerBuffer = 256);
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount,
            const AudioPlayer::PlaybackData &playbackData) override;
    void prepareAndProvidePlaybackData(PlaybackData *playbackData) override;
    virtual int getAudioDataSizeInBytes() = 0;
    virtual const char *provideAudioBuffer() = 0;
};


#endif //VOCALTRAINER_RAWPCMAUDIODATAPLAYER_H
