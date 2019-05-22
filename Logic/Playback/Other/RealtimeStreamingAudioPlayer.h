//
// Created by Semyon Tikhonenko on 9/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_REALTIMEAUDIOPLAYER_H
#define VOCALTRAINER_REALTIMEAUDIOPLAYER_H

#include "AudioPlayer.h"
#include <deque>
#include <vector>

class RealtimeStreamingAudioPlayer : public AudioPlayer {
public:
    explicit RealtimeStreamingAudioPlayer(const PlaybackData& playbackData);
    void pushAudioData(const void* data, int size);
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    void providePlaybackData(PlaybackData *playbackData) override;
    int getBufferSeek() const override;
private:
    std::deque<char> audioFeed;
    std::mutex mutex;
};


#endif //VOCALTRAINER_REALTIMEAUDIOPLAYER_H
