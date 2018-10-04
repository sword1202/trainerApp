//
// Created by Semyon Tikhonenko on 6/7/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOFILEPLAYER_H
#define VOCALTRAINER_AUDIOFILEPLAYER_H

#include "AudioPlayerWithDefaultSeekHandler.h"
#include "Decoder/audiodecoder.h"
#include "SoundTouch/SoundTouch.h"

class AudioFilePlayer : public AudioPlayerWithDefaultSeekHandler {
public:
    AudioFilePlayer();
    void setAudioData(std::string&& audioData);
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    void prepareAndProvidePlaybackData(PlaybackData *playbackData) override;

protected:
    void destroy() override;

private:
    AudioDecoder* audioDecoder = nullptr;
    soundtouch::SoundTouch soundTouch;
    std::vector<float> tempFloatBuffer;
    std::string audioData;
};


#endif //VOCALTRAINER_AUDIOFILEPLAYER_H
