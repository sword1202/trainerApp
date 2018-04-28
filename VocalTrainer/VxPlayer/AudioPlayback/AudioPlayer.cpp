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
    int size = self->readNextSamplesBatch(outputBuffer, self->playbackData);
    if (size == self->playbackData.framesPerBuffer) {
        return paContinue;
    } else {
        self->onComplete();
        return paComplete;
    }
}

void AudioPlayer::prepare() {
    prepareAndProvidePlaybackData(&playbackData);
    BOOST_ASSERT_MSG(playbackData.sampleRate > 0 &&
            playbackData.framesPerBuffer >= 0 &&
            playbackData.numChannels > 0 &&
            playbackData.totalStreamSamplesCount > 0,
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
    setSamplesCountSeek(secondsToSamplesCount(timeStamp));
}

double AudioPlayer::getTrackDurationInSeconds() {
    return samplesCountToSeconds(playbackData.totalStreamSamplesCount);
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
    return samplesCountToSeconds(getSamplesCountSeek());
}

int AudioPlayer::secondsToSamplesCount(double secondsSeek) const {
    return (int)round(secondsSeek * playbackData.sampleRate);
}

double AudioPlayer::samplesCountToSeconds(int samplesCount) const {
    return AudioUtils::GetSampleTimeInSeconds(samplesCount, playbackData.sampleRate);
}

void AudioPlayer::onComplete() {
    setSeek(0);
}
