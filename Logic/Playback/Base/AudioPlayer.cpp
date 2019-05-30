﻿//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "AudioPlayer.h"
#include <string>
#include "TimeUtils.h"
#include "PortAudioUtils.h"
#include "AudioUtils.h"
#include "Executors.h"

#include <boost/assert.hpp>
#include <cmath>

#include <stdexcept>
#include <iostream>
#include <cstring>

using namespace CppUtils;
using std::cout;
using std::endl;

int AudioPlayer::callback(
        const void *inputBuffer,
        void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData )
{
    auto* self = static_cast<AudioPlayer*>(userData);
    float volume = self->getVolume();

    memset(outputBuffer, 0, framesPerBuffer * self->getSampleSize());
    int framesCopiedToOutputBufferCount = self->readAudioDataApplySoundTouchIfNeed(outputBuffer, framesPerBuffer);

    assert(framesCopiedToOutputBufferCount <= (int)framesPerBuffer);
    int sampleSize = self->getSampleSize();
    // no data available, return silence and wait
    const PaSampleFormat format = self->playbackData.format;
    if (framesCopiedToOutputBufferCount < 0) {
        Executors::ExecuteOnMainThread([=] {
            self->onNoDataAvailableListeners.executeAll();
        });
        memset(outputBuffer, 0, framesPerBuffer * sampleSize);
        return paContinue;
    } else {
        if (volume == 0.0f) {
            auto sampleSize = self->getSampleSize();
            memset(outputBuffer, 0, sampleSize * framesCopiedToOutputBufferCount);
        } else if (volume != 1.0f) {
            int bufferSize = framesCopiedToOutputBufferCount * self->playbackData.numChannels;
            if (format == paInt16) {
                for (int i = 0; i < bufferSize; ++i) {
                    static_cast<int16_t*>(outputBuffer)[i] *= volume;
                }
            } else if (format == paInt32) {
                for (int i = 0; i < bufferSize; ++i) {
                    static_cast<int32_t*>(outputBuffer)[i] *= volume;
                }
            } else if (format == paInt8) {
                for (int i = 0; i < bufferSize; ++i) {
                    static_cast<int8_t*>(outputBuffer)[i] *= volume;
                }
            } else {
                throw std::runtime_error("Unsupported self->playbackData.format " + std::to_string(format));
            }
        }

        self->onDataSentToOutputListeners.executeAll(outputBuffer, framesCopiedToOutputBufferCount * sampleSize);

        if (framesCopiedToOutputBufferCount == framesPerBuffer) {
            return paContinue;
        } else {
            if (!self->looping) {
                self->onComplete();
                return paComplete;
            } else {
                self->setBufferSeek(0);
                return paContinue;
            }
        }
    }
}

void AudioPlayer::prepare() {
    assert(!isPrepared());
    providePlaybackData(&playbackData);

    if (playbackData.sampleRate <= 0 ||
            playbackData.framesPerBuffer < 0 ||
            playbackData.numChannels <= 0 ||
            playbackData.totalDurationInSeconds < 0) {
        throw std::runtime_error("AudioPlayer::prepare failed");
    }

    // Init soundtouch
    if (soundTouch) {
        soundTouch->setChannels((uint)playbackData.numChannels);
        soundTouch->setSampleRate((uint)playbackData.sampleRate);
        assert(playbackData.framesPerBuffer > 0 && playbackData.numChannels > 0);
        soundTouchTempFloatBuffer.resize(static_cast<size_t>(playbackData.framesPerBuffer) * playbackData.numChannels);
    }

    auto err = Pa_OpenDefaultStream( &stream,
            0,          /* no input channels */
            playbackData.numChannels, /* output channels */
            playbackData.format,
            playbackData.sampleRate,
            playbackData.framesPerBuffer,        /* frames per buffer, i.e. the number
                                                   of sample frames that PortAudio will
                                                   request from the callback. Many apps
                                                   may want to use
                                                   paFramesPerBufferUnspecified, which
                                                   tells PortAudio to pick the best,
                                                   possibly changing, buffer size.*/
            callback, /* this is your callback function */
            this); /*This is a pointer that will be passed to your callback*/

    PortAudio::checkErrors(err);

    onNoDataAvailableListeners.addListener([=] {
        onPlaybackStoppedListeners.executeAll();
    });
}

void AudioPlayer::play(double seek) {
    BOOST_ASSERT_MSG(isPrepared(), "call prepare before play");
    BOOST_ASSERT(seek >= 0);

    if (playing || dataSentToOutputListenerKey != 0) {
        return;
    }

    completed = false;
    setSeek(seek);

    setupPlaybackStartedListener();
    auto err=0;
    if (!Pa_IsStreamStopped(stream)) {
        err = Pa_StopStream(stream);
        PortAudio::checkErrors(err);
    }
    err = Pa_StartStream(stream);
    PortAudio::checkErrors(err);
}

bool AudioPlayer::isPlaying() const {
    return playing;
}

AudioPlayer::~AudioPlayer() {
    destroy();
}

void AudioPlayer::destroy() {
    if (!stream) {
        return;
    }

    playing = false;
    playbackData = PlaybackData();
    auto err = Pa_CloseStream(stream);
    PortAudio::checkErrors(err);
    stream = nullptr;
    delete soundTouch;
    soundTouch = nullptr;
}

void AudioPlayer::pause() {
    assert(isPrepared());
    if (!playing) {
        return;
    }

    Executors::ExecuteOnBackgroundThread([=] {
        pauseStream();
    });
}

void AudioPlayer::pauseStream() {
    auto err = Pa_AbortStream(stream);
    PortAudio::checkErrors(err);
    Executors::ExecuteOnMainThread([=] {
        playing = false;
        onDataSentToOutputListeners.removeListener(dataSentToOutputListenerKey);
        onPlaybackStoppedListeners.executeAll();
    });
}

void AudioPlayer::pauseSync() {
    assert(isPrepared());
    if (!playing) {
        return;
    }

    pauseStream();
}

void AudioPlayer::setSeek(double timeStamp) {
    assert(timeStamp >= 0);
    double durationInSeconds = getTrackDurationInSeconds();
    if (timeStamp > durationInSeconds) {
        timeStamp = durationInSeconds;
    }
    
    setBufferSeek(secondsSeekToBufferSeek(timeStamp));
}

double AudioPlayer::getTrackDurationInSeconds() const {
    return playbackData.totalDurationInSeconds;
}

AudioPlayer::AudioPlayer() {
    playing = false;
    volume = 1.0f;
    pitchShift = 0;
    tempoFactor = 1;
    looping = false;
    completed = false;
}

void AudioPlayer::play() {
    play(getSeek());
}

float AudioPlayer::getVolume() const {
    return volume;
}

void AudioPlayer::setVolume(float volume) {
    BOOST_ASSERT(volume >= 0.0f && volume <= 1.0f);
    this->volume = volume;
}

double AudioPlayer::getSeek() const {
    return bufferSeekToSecondsSeek(getBufferSeek());
}

int AudioPlayer::secondsToSamplesCount(double secondsSeek) const {
    return (int)round(secondsSeek * playbackData.sampleRate);
}

double AudioPlayer::samplesCountToSeconds(int samplesCount) const {
    return AudioUtils::GetSampleTimeInSeconds(samplesCount, playbackData.sampleRate);
}

void AudioPlayer::onComplete() {
    playing = false;
    completed = true;
    setSeek(0);

    Executors::ExecuteOnMainThread([=] {
        onPlaybackStoppedListeners.executeAll();
        onCompleteListeners.executeAll();
    });
}

int AudioPlayer::getSampleSize() const {
    return Pa_GetSampleSize(playbackData.format) * playbackData.numChannels;
}

double AudioPlayer::bufferSeekToSecondsSeek(int bufferSeek) const {
    return samplesCountToSeconds(bufferSeek);
}

int AudioPlayer::secondsSeekToBufferSeek(double timestamp) const {
    return secondsToSamplesCount(timestamp);
}

void AudioPlayer::setBufferSeek(int bufferSeek) {
    double seek = bufferSeekToSecondsSeek(bufferSeek);
    double total = getTrackDurationInSeconds();
    seekChangedListeners.executeAll(seek, total);
}

void AudioPlayer::prepareAsync(const std::function<void()>& callback) {
    Executors::ExecuteOnBackgroundThread([=] {
        prepare();
        Executors::ExecuteOnMainThread(callback);
    });
}

int AudioPlayer::getPitchShiftInSemiTones() const {
    return pitchShift;
}

void AudioPlayer::setPitchShiftInSemiTones(int value) {
    if (value == pitchShift) {
        return;
    }

    pitchShift = value;
    onTonalityChanged(value);
}

const PlaybackData &AudioPlayer::getPlaybackData() const {
    return playbackData;
}

void AudioPlayer::setupPlaybackStartedListener() {
    assert(dataSentToOutputListenerKey == 0);
    dataSentToOutputListenerKey = onDataSentToOutputListeners.addOneShotListener([=] (void*, int) {
        Executors::ExecuteOnMainThread([this] {
            playing = true;
            onPlaybackStartedListeners.executeAll();
            dataSentToOutputListenerKey = 0;
        });
    });
}

bool AudioPlayer::isPrepared() const {
    return stream != nullptr;
}

double AudioPlayer::getTempoFactor() const {
    return tempoFactor;
}

void AudioPlayer::setTempoFactor(double tempoFactor) {
    this->tempoFactor = tempoFactor;
}

double AudioPlayer::getCallbackBufferDurationInSeconds() const {
    return AudioUtils::GetSampleTimeInSeconds(playbackData.framesPerBuffer, playbackData.sampleRate);
}

bool AudioPlayer::isLooping() const {
    return looping;
}

void AudioPlayer::setLooping(bool looping) {
    this->looping = looping;
}

void AudioPlayer::setTotalDurationInSeconds(double totalDurationInSeconds) {
    playbackData.totalDurationInSeconds = totalDurationInSeconds;
}

void AudioPlayer::setPlaybackData(const PlaybackData &playbackData) {
    this->playbackData = playbackData;
}

bool AudioPlayer::isCompleted() const {
    return completed;
}

const std::string &AudioPlayer::getPlayerName() const {
    return playerName;
}

void AudioPlayer::setPlayerName(const std::string &playerName) {
    this->playerName = playerName;
}

void AudioPlayer::initSoundTouch() {
    assert(!soundTouch && "SoundTouch already initialised");
    soundTouch = new soundtouch::SoundTouch();
}

void AudioPlayer::onTonalityChanged(int value) {
    assert(pitchShift == 0 || soundTouch && "tonality changes are not allowed, soundtouch not "
                                            "initialised, call initSoundTouch() before prepare to use pitch shifting");
    soundTouch->setPitchSemiTones(value);
}

int AudioPlayer::readAudioDataApplySoundTouchIfNeed(void *outputBuffer, int requestedFramesCount) {
    // Apply tempo and tonality changes, if need.
    if (soundTouch && (pitchShift != 0 || tempoFactor != 1)) {
        auto* samplesData = static_cast<int16_t *>(outputBuffer);

        while (soundTouch->numSamples() < requestedFramesCount) {
            int readFramesCount = readNextSamplesBatch(outputBuffer, requestedFramesCount, playbackData);
            int dataArraySize = readFramesCount * playbackData.numChannels;
            AudioUtils::Int16SamplesIntoFloatSamples(samplesData,
                                                     dataArraySize,
                                                     soundTouchTempFloatBuffer.data());
            soundTouch->putSamples(soundTouchTempFloatBuffer.data(), uint(readFramesCount));

            // End of a track reached
            if (readFramesCount != requestedFramesCount) {
                break;
            }
        }

        int readFramesCount = std::min(soundTouch->numSamples(), uint(requestedFramesCount));
        int dataArraySize = readFramesCount * playbackData.numChannels;
        soundTouch->receiveSamples(soundTouchTempFloatBuffer.data(), uint(readFramesCount));
        AudioUtils::FloatSamplesIntoInt16Samples(soundTouchTempFloatBuffer.data(), dataArraySize, samplesData);
        return readFramesCount;
    } else {
        return readNextSamplesBatch(outputBuffer, requestedFramesCount, playbackData);
    }
}
