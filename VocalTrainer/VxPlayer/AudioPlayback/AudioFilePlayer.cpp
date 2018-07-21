//
// Created by Semyon Tikhonenko on 6/7/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "AudioFilePlayer.h"
#include "AudioUtils.h"

#define SEEK_LOCK std::lock_guard<std::mutex> _(bufferSeekMutex)

int AudioFilePlayer::readNextSamplesBatch(void *intoBuffer, int framesCount, const AudioPlayer::PlaybackData &playbackData) {
    int bufferSeekBefore = getBufferSeek();
    audioDecoder->seek(bufferSeekBefore * playbackData.numChannels);
    assert(audioDecoder->positionInSamples() == bufferSeekBefore * playbackData.numChannels);
    int samplesCount = framesCount * playbackData.numChannels;
    int readFramesCount = audioDecoder->read(samplesCount, (short*)intoBuffer)
            / playbackData.numChannels;
    if (readFramesCount > 0) {
        // handle a case, when seek is set from outside while audioDecoder is reading the data
        SEEK_LOCK;
        if (bufferSeekBefore == this->bufferSeek) {
            this->bufferSeek = bufferSeek + readFramesCount;
            AudioPlayer::setBufferSeek(bufferSeek);
        }
    }

    int shiftInSemiTones = getPitchShiftInSemiTones();
    if (shiftInSemiTones != 0) {
        soundTouch.setPitchSemiTones(shiftInSemiTones);
        AudioUtils::Int16SamplesIntoFloatSamples((short*)intoBuffer, samplesCount, tempFloatBuffer.data());
        soundTouch.putSamples(tempFloatBuffer.data(), (uint)framesCount);
        soundTouch.receiveSamples(tempFloatBuffer.data(), (uint)framesCount);
        AudioUtils::FloatSamplesIntoInt16Samples(tempFloatBuffer.data(), samplesCount, (short*)intoBuffer);
    }

    return readFramesCount;
}

void AudioFilePlayer::prepareAndProvidePlaybackData(AudioPlayer::PlaybackData *playbackData) {
    assert(!audioDecoder);
    audioDecoder = AudioDecoder::create();
    audioDecoder->open(std::move(audioData));
    playbackData->numChannels = audioDecoder->channels();
    playbackData->format = paInt16;
    playbackData->sampleRate = audioDecoder->sampleRate();
    playbackData->framesPerBuffer = 256;
    playbackData->totalDurationInSeconds = audioDecoder->duration();
    
    soundTouch.setChannels((uint)playbackData->numChannels);
    soundTouch.setSampleRate((uint)playbackData->sampleRate);
    tempFloatBuffer.resize(playbackData->framesPerBuffer * playbackData->numChannels);
}

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

AudioFilePlayer::AudioFilePlayer() {

}

void AudioFilePlayer::setAudioData(std::string &&audioData) {
    destroy();
    this->audioData.swap(audioData);
    setBufferSeek(0);
}

void AudioFilePlayer::destroy() {
    AudioPlayer::destroy();
    if (audioDecoder) {
        delete audioDecoder;
    }
}
