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

    int shiftInSemiTones = getPitchShiftInSemiTones();
    if (shiftInSemiTones != 0) {

    }

    return readFramesCount;
}

void AudioFilePlayer::prepareAndProvidePlaybackData(PlaybackData *playbackData) {
    assert(!audioDecoder);
    audioDecoder = AudioDecoder::create();
    audioDecoder->open(&audioData);
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

void AudioFilePlayer::setAudioData(std::string &&audioData) {
    destroy();
    this->audioData = std::move(audioData);
    setBufferSeek(0);
}

void AudioFilePlayer::destroy() {
    AudioPlayer::destroy();
    if (audioDecoder) {
        delete audioDecoder;
    }
}

const std::string &AudioFilePlayer::getAudioData() const {
    return audioData;
}
