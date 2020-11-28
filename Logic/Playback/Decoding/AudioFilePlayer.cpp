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

    if (audioDecoder->positionInSamples() != bufferSeekBefore * playbackData.numChannels) {
        audioDecoder->seek(bufferSeekBefore * playbackData.numChannels);
        assert (audioDecoder->positionInSamples() == bufferSeekBefore * playbackData.numChannels);
    }

    int samplesCount = framesCount * playbackData.numChannels;
    int readFramesCount = audioDecoder->read(samplesCount, (short*)intoBuffer)
            / playbackData.numChannels;
    if (readFramesCount > 0) {
        moveBufferSeekIfNotChangedBefore(readFramesCount, bufferSeekBefore);
    }

    return readFramesCount;
}

void AudioFilePlayer::providePlaybackData(PlaybackData *playbackData) {
    assert(!audioDecoder);
    audioDecoder = AudioDecoder::create();
    audioDecoder->open(audioData);
    playbackData->numChannels = audioDecoder->channels();
    playbackData->format = paInt16;
    playbackData->sampleRate = audioDecoder->sampleRate();
    playbackData->framesPerBuffer = 256;
    playbackData->totalDurationInSeconds = audioDecoder->duration();
}

AudioFilePlayer::AudioFilePlayer() {
    setPlayerName("AudioFilePlayer");
    initSoundTouch();
}

void AudioFilePlayer::setAudioData(const AudioData* audioData) {
    destroy();
    this->audioData = audioData;
    setBufferSeek(0);
}

void AudioFilePlayer::destroy() {
    PortAudioPlayer::destroy();
    if (audioDecoder) {
        delete audioDecoder;
    }
}

const AudioData* AudioFilePlayer::getAudioData() const {
    return audioData;
}
