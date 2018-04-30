//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "AudioFilePlayer.h"
#include "Strings.h"
#include "WAVFile.h"
#include "WavAudioPlayer.h"
#include "Mp3AudioPlayer.h"

#define SEEK_LOCK std::lock_guard<std::mutex> _(seekMutex)

using namespace CppUtils;

int AudioFilePlayer::getBufferSeek() const {
    SEEK_LOCK;
    return bufferSeek;
}

void AudioFilePlayer::setBufferSeek(int bufferSeek) {
    SEEK_LOCK;
    this->bufferSeek = bufferSeek;
}

AudioFilePlayer::AudioFilePlayer(std::string &&audioData) : audioData(std::move(audioData)) {

}

void AudioFilePlayer::setBufferSeekIfNotModified(int bufferSeek, int lastBufferSeek) {
    SEEK_LOCK;
    if (lastBufferSeek == this->bufferSeek) {
        this->bufferSeek = bufferSeek;
    }
}

AudioFilePlayer *AudioFilePlayer::create(std::istream &is) {
     std::string audioData = Strings::StreamToString(is);
     if (WAVFile::isWavFile(audioData.data(), audioData.size())) {
         return new WavAudioPlayer(std::move(audioData));
     } else {
         return new Mp3AudioPlayer(std::move(audioData));
     }
}
