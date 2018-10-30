//
// Created by Semyon Tikhonenko on 9/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <cstring>

#include "RealtimeStreamingAudioPlayer.h"
#include "NotImplementedAssert.h"

#define LOCK std::lock_guard<std::mutex> _(mutex)

int RealtimeStreamingAudioPlayer::readNextSamplesBatch(void *intoBuffer, int framesCount, const AudioPlayer::PlaybackData &playbackData) {
    size_t bufferSize = (size_t)framesCount * getSampleSize();
    if (mutex.try_lock()) {
        size_t availableFeedSize = std::min(bufferSize, audioFeed.size());
        auto begin = audioFeed.begin();
        auto end = audioFeed.begin() + availableFeedSize;
        std::copy(begin, end, (char*)intoBuffer);
        audioFeed.erase(begin, end);
        memset((char*)intoBuffer + availableFeedSize, 0, bufferSize - availableFeedSize);
        mutex.unlock();
    } else {
        return -1;
    }

    return framesCount;
}

void RealtimeStreamingAudioPlayer::prepareAndProvidePlaybackData(AudioPlayer::PlaybackData *playbackData) {
    if (playbackData->totalDurationInSeconds <= 0) {
        playbackData->totalDurationInSeconds = INT32_MAX;
    }
}

int RealtimeStreamingAudioPlayer::getBufferSeek() const {
    return 0;
}

RealtimeStreamingAudioPlayer::RealtimeStreamingAudioPlayer(const PlaybackData& playbackData) {
    setPlaybackData(playbackData);
}

void RealtimeStreamingAudioPlayer::pushAudioData(const void *data, int size) {
    char* charData = (char*)data;
    LOCK;
    audioFeed.insert(audioFeed.end(), charData, charData + size);
}
