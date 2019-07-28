//
// Created by Semyon Tikhonenko on 6/7/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOFILEPLAYER_H
#define VOCALTRAINER_AUDIOFILEPLAYER_H

#include "AudioPlayerWithDefaultSeekHandler.h"
#include "audiodecoder.h"
#include "SoundTouch/SoundTouch.h"

class AudioFilePlayer : public AudioPlayerWithDefaultSeekHandler {
public:
    AudioFilePlayer();
    void setAudioData(const std::string* audioData);
    const std::string* getAudioData() const;
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    void providePlaybackData(PlaybackData *playbackData) override;

protected:
    void destroy() override;

private:
    AudioDecoder* audioDecoder = nullptr;
    const std::string* audioData = nullptr;
};


#endif //VOCALTRAINER_AUDIOFILEPLAYER_H
