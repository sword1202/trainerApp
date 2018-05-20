//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "AudioFilePlayer.h"
#include "Strings.h"
#include "WAVFile.h"
#include "WavAudioPlayer.h"
#include "Mp3AudioPlayer.h"
#include "Streams.h"

#define SEEK_LOCK std::lock_guard<std::mutex> _(seekMutex)

using namespace CppUtils;

int AudioFilePlayer::getBufferSeek() const {
    SEEK_LOCK;
    return bufferSeek;
}

void AudioFilePlayer::setBufferSeek(int bufferSeek) {
    {
        SEEK_LOCK;
        this->bufferSeek = bufferSeek;
    }
    AudioPlayer::setBufferSeek(bufferSeek);
}

AudioFilePlayer::AudioFilePlayer(std::string &&audioData) : audioData(std::move(audioData)) {

}

void AudioFilePlayer::setBufferSeekIfNotModified(int bufferSeek, int lastBufferSeek) {
    SEEK_LOCK;
    if (lastBufferSeek == this->bufferSeek) {
        this->bufferSeek = bufferSeek;
        AudioPlayer::setBufferSeek(bufferSeek);
    }
}

AudioFilePlayer *AudioFilePlayer::create(std::istream &is) {
    std::string audioData = Strings::StreamToString(is);
    return create(std::move(audioData));
}

AudioFilePlayer *AudioFilePlayer::create(const char *filePath) {
    std::fstream is = Streams::OpenFile(filePath, std::ios::in | std::ios::binary);
    return create(is);
}

AudioFilePlayer *AudioFilePlayer::create(std::string &&audioData) {
    if (WAVFile::isWavFile(audioData.data(), audioData.size())) {
        return new WavAudioPlayer(std::move(audioData));
    } else {
        return new Mp3AudioPlayer(std::move(audioData));
    }
}
