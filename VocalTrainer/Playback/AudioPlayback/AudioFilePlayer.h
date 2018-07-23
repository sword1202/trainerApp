//
// Created by Semyon Tikhonenko on 6/7/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOFILEPLAYER_H
#define VOCALTRAINER_AUDIOFILEPLAYER_H

#include "AudioPlayer.h"
#include "Decoder/audiodecoder.h"
#include "SoundTouch/SoundTouch.h"

class AudioFilePlayer : public AudioPlayer {
public:
    AudioFilePlayer();
    void setAudioData(std::string&& audioData);
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    void prepareAndProvidePlaybackData(PlaybackData *playbackData) override;

protected:
    int getBufferSeek() const override;
    void setBufferSeek(int bufferSeek) override;
    void destroy() override;

private:
    AudioDecoder* audioDecoder = nullptr;
    soundtouch::SoundTouch soundTouch;
    std::vector<float> tempFloatBuffer;
    std::string audioData;
    int bufferSeek = 0;
    mutable std::mutex bufferSeekMutex;
};


#endif //VOCALTRAINER_AUDIOFILEPLAYER_H
