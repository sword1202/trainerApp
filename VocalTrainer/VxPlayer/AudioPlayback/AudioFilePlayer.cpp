//
// Created by Semyon Tikhonenko on 6/7/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "AudioFilePlayer.h"

#define SEEK_LOCK std::lock_guard<std::mutex> _(bufferSeekMutex)

int AudioFilePlayer::readNextSamplesBatch(void *intoBuffer, int framesCount, const AudioPlayer::PlaybackData &playbackData) {
    int bufferSeekBefore = getBufferSeek();
    audioDecoder->seek(bufferSeekBefore * playbackData.numChannels);
    assert(audioDecoder->positionInSamples() == bufferSeekBefore * playbackData.numChannels);
    int readFramesCount = audioDecoder->read(framesCount * playbackData.numChannels, (SAMPLE*)intoBuffer)
            / playbackData.numChannels;
    if (readFramesCount > 0) {
        // handle a case, when seek is set from outside while audioDecoder is reading the data
        SEEK_LOCK;
        if (bufferSeekBefore == this->bufferSeek) {
            this->bufferSeek = bufferSeek + readFramesCount;
            AudioPlayer::setBufferSeek(bufferSeek);
        }
    }

    return readFramesCount;
}

void AudioFilePlayer::prepareAndProvidePlaybackData(AudioPlayer::PlaybackData *playbackData) {
    if (audioDecoder) {
        delete audioDecoder;
    }

    audioDecoder = AudioDecoder::create();
    audioDecoder->open(std::move(audioData));
    playbackData->numChannels = audioDecoder->channels();
    playbackData->format = paInt16;
    playbackData->sampleRate = audioDecoder->sampleRate();
    playbackData->framesPerBuffer = paFramesPerBufferUnspecified;
    playbackData->totalDurationInSeconds = audioDecoder->duration();
}

int AudioFilePlayer::getBufferSeek() const {
    SEEK_LOCK;
    return bufferSeek;
}

AudioFilePlayer::~AudioFilePlayer() {
    if (audioDecoder) {
        delete audioDecoder;
    }
}

AudioFilePlayer::AudioFilePlayer(std::string &&audioData) : audioData(std::move(audioData)) {

}

void AudioFilePlayer::setBufferSeek(int bufferSeek) {
    {
        SEEK_LOCK;
        this->bufferSeek = bufferSeek;
    }
    AudioPlayer::setBufferSeek(bufferSeek);
}
