//
// Created by Semyon Tikhonenko on 6/7/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "AudioFilePlayer.h"
#include "AudioUtils.h"
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;

int AudioFilePlayer::readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) {
    int bufferSeekBefore = getBufferSeek();

    if (audioDecoder->positionInSamples() != bufferSeekBefore * playbackData.numberOfChannels) {
        audioDecoder->seek(bufferSeekBefore * playbackData.numberOfChannels);
        assert (audioDecoder->positionInSamples() == bufferSeekBefore * playbackData.numberOfChannels);
    }

    int samplesCount = framesCount * playbackData.numberOfChannels;
    int readFramesCount = audioDecoder->read(samplesCount, (short*)intoBuffer)
            / playbackData.numberOfChannels;
    if (readFramesCount > 0) {
        moveBufferSeekIfNotChangedBefore(readFramesCount, bufferSeekBefore);
    }

    return readFramesCount;
}

void AudioFilePlayer::providePlaybackData(PlaybackData *playbackData) {
    assert(!audioDecoder);
    audioDecoder = AudioDecoder::create();
    audioDecoder->open(audioData);
    playbackData->numberOfChannels = static_cast<unsigned int>(audioDecoder->channels());
    playbackData->bitsPerChannel = 16;
    playbackData->sampleRate = static_cast<unsigned int>(audioDecoder->sampleRate());
    playbackData->samplesPerBuffer = 256;
    playbackData->totalDurationInSeconds = audioDecoder->duration();
}

AudioFilePlayer::AudioFilePlayer() {
    setPlayerName("AudioFilePlayer");
    initSoundTouch();
}

void AudioFilePlayer::setAudioData(AudioDataBufferConstPtr audioData) {
    reset();
    this->audioData = audioData;
    setBufferSeek(0);
}

void AudioFilePlayer::reset() {
    BaseAudioPlayer::reset();
    delete audioDecoder;
    audioDecoder = nullptr;
}

const AudioDataBuffer* AudioFilePlayer::getAudioData() const {
    return audioData.get();
}
