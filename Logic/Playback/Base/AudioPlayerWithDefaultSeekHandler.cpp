//
// Created by Semyon Tikhonenko on 7/23/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "AudioPlayerWithDefaultSeekHandler.h"

#define SEEK_LOCK std::lock_guard<std::mutex> _(bufferSeekMutex)

int AudioPlayerWithDefaultSeekHandler::getBufferSeek() const {
    SEEK_LOCK;
    return bufferSeek;
}

void AudioPlayerWithDefaultSeekHandler::setBufferSeek(int bufferSeek) {
    int seekBefore;
    {
        SEEK_LOCK;
        seekBefore = this->bufferSeek;
        this->bufferSeek = bufferSeek;
        onBufferSeekChanged(seekBefore, bufferSeek);
    }
    BaseAudioPlayer::setBufferSeek(bufferSeek);
}

void AudioPlayerWithDefaultSeekHandler::moveBufferSeekIfNotChangedBefore(int moveBy, int seekBefore) {
    bufferSeekMutex.lock();
    if (seekBefore == this->bufferSeek) {
        this->bufferSeek = bufferSeek + moveBy;
        onBufferSeekChanged(seekBefore, this->bufferSeek);
        bufferSeekMutex.unlock();
        BaseAudioPlayer::setBufferSeek(bufferSeek);
    } else {
        bufferSeekMutex.unlock();
    }
}

void AudioPlayerWithDefaultSeekHandler::onBufferSeekChanged(int before, int now) {

}
