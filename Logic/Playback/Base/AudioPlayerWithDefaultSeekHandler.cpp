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
    {
        SEEK_LOCK;
        this->bufferSeek = bufferSeek;
    }
    PortAudioPlayer::setBufferSeek(bufferSeek);
}

void AudioPlayerWithDefaultSeekHandler::moveBufferSeekIfNotChangedBefore(int moveBy, int seekBefore) {
    bool changed = false;
    SEEK_LOCK;
    {
        if (seekBefore == this->bufferSeek) {
            this->bufferSeek = bufferSeek + moveBy;
            changed = true;
        }
    }
    if (changed) {
        PortAudioPlayer::setBufferSeek(bufferSeek);
    }
}
