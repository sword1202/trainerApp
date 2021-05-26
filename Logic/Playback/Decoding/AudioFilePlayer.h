//
// Created by Semyon Tikhonenko on 6/7/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOFILEPLAYER_H
#define VOCALTRAINER_AUDIOFILEPLAYER_H

#include "AudioPlayerWithDefaultSeekHandler.h"
#include "audiodecoder.h"
#include "AudioDataBuffer.h"
#include "SoundTouch/SoundTouch.h"

class AudioFilePlayer : public AudioPlayerWithDefaultSeekHandler {
public:
    AudioFilePlayer();
    void setAudioData(AudioDataBufferConstPtr audioData);
    const AudioDataBuffer* getAudioData() const;
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    void providePlaybackData(PlaybackData *playbackData) override;

protected:
    void destroy() override;

private:
    AudioDecoder* audioDecoder = nullptr;
    AudioDataBufferConstPtr audioData;
};


#endif //VOCALTRAINER_AUDIOFILEPLAYER_H
