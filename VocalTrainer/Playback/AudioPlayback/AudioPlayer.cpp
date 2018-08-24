//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import <stdexcept>
#include "AudioPlayer.h"
#include <string>
#include "TimeUtils.h"
#include "PortAudio.h"
#include "AudioUtils.h"
#include "Executors.h"

#import <iostream>
#include <boost/assert.hpp>
#include <cmath>

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
    AudioPlayer* self = (AudioPlayer*)userData;
    float volume = self->getVolume();

    int readFramesCount = self->readNextSamplesBatch(outputBuffer, framesPerBuffer, self->playbackData);
    assert(readFramesCount <= (int)framesPerBuffer);
    // no data available, return silence and wait
    const PaSampleFormat format = self->playbackData.format;
    if (readFramesCount < 0) {
        Executors::ExecuteOnMainThread([=] {
            self->onNoDataAvailableListeners.executeAll();
        });
        int sampleSize = self->getSampleSize();
        memset(outputBuffer, 0, framesPerBuffer * sampleSize);
        return paContinue;
    } else {
        if (volume == 0.0f) {
            auto sampleSize = self->getSampleSize();
            memset(outputBuffer, 0, sampleSize * readFramesCount);
        } else if (volume != 1.0f) {
            int bufferSize = readFramesCount * self->playbackData.numChannels;
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

        Executors::ExecuteOnMainThread([=] {
            self->onDataSentToOutputListeners.executeAll(outputBuffer, readFramesCount);
        });

        if (readFramesCount == framesPerBuffer) {
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
    prepareAndProvidePlaybackData(&playbackData);
    BOOST_ASSERT_MSG(playbackData.sampleRate > 0 &&
            playbackData.framesPerBuffer >= 0 &&
            playbackData.numChannels > 0 &&
            playbackData.totalDurationInSeconds >= 0,
            "not all playback data provided");

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

        return DONT_DELETE_LISTENER;
    });
}

void AudioPlayer::play(double seek) {
    BOOST_ASSERT_MSG(isPrepared(), "call prepare before play");
    BOOST_ASSERT(seek >= 0);

    if (playing || dataSentToOutputListenerKey != 0) {
        return;
    }

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
    assert(timeStamp >= 0 && timeStamp <= getTrackDurationInSeconds());
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
    setSeek(0);

    Executors::ExecuteOnMainThread([=] {
        onPlaybackStoppedListeners.executeAll();
        onCompleteListeners.executeAll();
    });
}

int AudioPlayer::getSampleSize() const {
    return Pa_GetSampleSize(playbackData.format) * playbackData.numChannels;
}

void AudioPlayer::playFromSeekToSeek(double a, double b, const std::function<void()> onFinish) {
    assert(a < b);
    assert(a > 0 && b > 0);
    assert(b <= getTrackDurationInSeconds());

    play(a);
    seekChangedListeners.addListener([=] (double seek, double _) {
        if (seek >= b) {
            pause();
            onFinish();
            return DELETE_LISTENER;
        }

        return DONT_DELETE_LISTENER;
    });
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
    Executors::ExecuteOnMainThread([=] {
        seekChangedListeners.executeAll(seek, total);
    });
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
    pitchShift = value;
}

const AudioPlayer::PlaybackData &AudioPlayer::getPlaybackData() const {
    return playbackData;
}

void AudioPlayer::setupPlaybackStartedListener() {
    assert(dataSentToOutputListenerKey == 0);
    dataSentToOutputListenerKey = onDataSentToOutputListeners.addListener([=] (void*, int) {
        playing = true;
        onPlaybackStartedListeners.executeAll();
        dataSentToOutputListenerKey = 0;
        return DELETE_LISTENER;
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
