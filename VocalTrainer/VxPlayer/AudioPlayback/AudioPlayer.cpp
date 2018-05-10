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
        int sampleSize = self->getSampleSize();
        memset(outputBuffer, 0, readFramesCount * sampleSize);
        return paContinue;
    } else {
        if (volume == 0.0f) {
            auto sampleSize = Pa_GetSampleSize(format) * self->playbackData.numChannels;
            memset(outputBuffer, 0, readFramesCount * sampleSize);
        } else if (volume != 1.0f) {
            if (format == paInt16) {
                for (int i = 0; i < readFramesCount; ++i) {
                    static_cast<int16_t*>(outputBuffer)[i] *= volume;
                }
            } else if (format == paInt32) {
                for (int i = 0; i < readFramesCount; ++i) {
                    static_cast<int32_t*>(outputBuffer)[i] *= volume;
                }
            } else if (format == paInt8) {
                for (int i = 0; i < readFramesCount; ++i) {
                    static_cast<int8_t*>(outputBuffer)[i] *= volume;
                }
            } else {
                std::runtime_error("Unsupported self->playbackData.format " + std::to_string(format));
            }
        }

        if (readFramesCount == framesPerBuffer) {
            return paContinue;
        } else {
            Executors::ExecuteOnMainThread([self](){ self->onComplete(); });
            return paComplete;
        }
    }
}

void AudioPlayer::prepare() {
    prepareAndProvidePlaybackData(&playbackData);
    BOOST_ASSERT_MSG(playbackData.sampleRate > 0 &&
            playbackData.framesPerBuffer >= 0 &&
            playbackData.numChannels > 0 &&
            playbackData.totalDurationInSeconds > 0,
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
}

void AudioPlayer::play(double seek) {
    BOOST_ASSERT_MSG(playbackData.sampleRate > 0, "call prepare before play");
    BOOST_ASSERT(seek >= 0);

    setSeek(seek);
    playing = true;

    auto err = Pa_StartStream(stream);
    PortAudio::checkErrors(err);
}

bool AudioPlayer::isPlaying() const {
    return playing;
}

AudioPlayer::~AudioPlayer() {
    auto err = Pa_CloseStream(stream);
    PortAudio::checkErrors(err);
}

void AudioPlayer::pause() {
    playing = false;
    auto err = Pa_AbortStream(stream);
    PortAudio::checkErrors(err);
}

void AudioPlayer::setSeek(double timeStamp) {
    assert(timeStamp >= 0 && timeStamp <= getTrackDurationInSeconds());
    setBufferSeek(secondsSeekToBufferSeek(timeStamp));
}

double AudioPlayer::getTrackDurationInSeconds() {
    return playbackData.totalDurationInSeconds;
}

AudioPlayer::AudioPlayer() {

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
    setSeek(0);
    onCompleteListeners.executeAll();
}

int AudioPlayer::getSampleSize() const {
    return Pa_GetSampleSize(playbackData.format) * playbackData.numChannels;
}

void AudioPlayer::playFromSeekToSeek(double a, double b, const std::function<void()> onFinish) {
    assert(a < b);
    assert(a > 0 && b > 0);
    assert(b <= getTrackDurationInSeconds());

    play(a);
    addSeekChangedListener([=] (double seek, double _) {
        if (seek >= b) {
            pause();
            onFinish();
            return DELETE_LISTENER;
        }

        return DONT_DELETE_LISTENER;
    });
}

int AudioPlayer::addOnCompleteListener(const OnCompleteListener &listener) {
    return onCompleteListeners.addListener(listener);
}

void AudioPlayer::removeOnCompleteListener(int key) {
    onCompleteListeners.removeListener(key);
}

int AudioPlayer::addSeekChangedListener(const AudioPlayer::SeekChangedListener &listener) {
    return seekChangedListeners.addListener(listener);
}

void AudioPlayer::removeSeekChangedListener(int key) {
    seekChangedListeners.removeListener(key);
}

double AudioPlayer::bufferSeekToSecondsSeek(int bufferSeek) const {
    return samplesCountToSeconds(bufferSeek);
}

int AudioPlayer::secondsSeekToBufferSeek(double timestamp) const {
    return secondsToSamplesCount(timestamp);
}

void AudioPlayer::setBufferSeek(int bufferSeek) {
    Executors::ExecuteOnMainThread([=] {
        double seek = bufferSeekToSecondsSeek(bufferSeek);
        double total = getTrackDurationInSeconds();
        seekChangedListeners.executeAll(seek, total);
    });
}

void AudioPlayer::prepareAsync(const std::function<void()>& callback) {
    Executors::ExecuteOnBackgroundThread([=] {
        prepare();
        Executors::ExecuteOnMainThread(callback);
    });
}

void AudioPlayer::destroy(const std::function<void()>& onDestroyed) {
    delete this;
    if (onDestroyed) {
        onDestroyed();
    }
}

void AudioPlayer::destroy() {
    destroy(nullptr);
}
