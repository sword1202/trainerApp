//
// Created by Semyon Tikhonenko on 6/7/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOFILEPLAYER_H
#define VOCALTRAINER_AUDIOFILEPLAYER_H

#include "AudioPlayer.h"
#include "Decoder/audiodecoder.h"

class AudioFilePlayer : public AudioPlayer {
public:
    AudioFilePlayer(std::string&& audioData);
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    void prepareAndProvidePlaybackData(PlaybackData *playbackData) override;
    int getBufferSeek() const override;
    ~AudioFilePlayer();
private:
    AudioDecoder* audioDecoder = nullptr;
    std::string audioData;
};


#endif //VOCALTRAINER_AUDIOFILEPLAYER_H
