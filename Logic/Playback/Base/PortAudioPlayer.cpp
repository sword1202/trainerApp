//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "PortAudioPlayer.h"
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

int PortAudioPlayer::callback(
        const void *inputBuffer,
        void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData )
{
    auto* self = static_cast<PortAudioPlayer*>(userData);
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

void PortAudioPlayer::prepare() {
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

void PortAudioPlayer::play(double seek) {
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

bool PortAudioPlayer::isPlaying() const {
    return playing;
}

PortAudioPlayer::~PortAudioPlayer() {
    destroy();
}

void PortAudioPlayer::destroy() {
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

void PortAudioPlayer::pause() {
    assert(isPrepared());
    if (!playing) {
        return;
    }

    Executors::ExecuteOnBackgroundThread([=] {
        pauseStream();
    });
}

void PortAudioPlayer::pauseStream() {
    auto err = Pa_AbortStream(stream);
    PortAudio::checkErrors(err);
    Executors::ExecuteOnMainThread([=] {
        playing = false;
        onDataSentToOutputListeners.removeListener(dataSentToOutputListenerKey);
        onPlaybackStoppedListeners.executeAll();
    });
}

void PortAudioPlayer::pauseSync() {
    assert(isPrepared());
    if (!playing) {
        return;
    }

    pauseStream();
}

void PortAudioPlayer::setSeek(double timeStamp) {
    assert(timeStamp >= 0);
    double durationInSeconds = getTrackDurationInSeconds();
    if (timeStamp > durationInSeconds) {
        timeStamp = durationInSeconds;
    }

    if (timeStamp < 0) {
        timeStamp = 0;
    }
    
    setBufferSeek(secondsSeekToBufferSeek(timeStamp));
}

double PortAudioPlayer::getTrackDurationInSeconds() const {
    return playbackData.totalDurationInSeconds;
}

PortAudioPlayer::PortAudioPlayer() {
    playing = false;
    volume = 1.0f;
    pitchShift = 0;
    tempoFactor = 1;
    looping = false;
    completed = false;
}

float PortAudioPlayer::getVolume() const {
    return volume;
}

void PortAudioPlayer::setVolume(float volume) {
    BOOST_ASSERT(volume >= 0.0f && volume <= 1.0f);
    this->volume = volume;
}

double PortAudioPlayer::getSeek() const {
    return bufferSeekToSecondsSeek(getBufferSeek());
}

int PortAudioPlayer::secondsToSamplesCount(double secondsSeek) const {
    return (int)round(secondsSeek * playbackData.sampleRate);
}

double PortAudioPlayer::samplesCountToSeconds(int samplesCount) const {
    return AudioUtils::GetSampleTimeInSeconds(samplesCount, playbackData.sampleRate);
}

void PortAudioPlayer::onComplete() {
    playing = false;
    completed = true;
    setSeek(0);

    Executors::ExecuteOnMainThread([=] {
        onPlaybackStoppedListeners.executeAll();
        onCompleteListeners.executeAll();
    });
}

int PortAudioPlayer::getSampleSize() const {
    return Pa_GetSampleSize(playbackData.format) * playbackData.numChannels;
}

double PortAudioPlayer::bufferSeekToSecondsSeek(int bufferSeek) const {
    return samplesCountToSeconds(bufferSeek);
}

int PortAudioPlayer::secondsSeekToBufferSeek(double timestamp) const {
    return secondsToSamplesCount(timestamp);
}

void PortAudioPlayer::setBufferSeek(int bufferSeek) {
    double seek = bufferSeekToSecondsSeek(bufferSeek);
    double total = getTrackDurationInSeconds();
    seekChangedListeners.executeAll(seek, total);
}

int PortAudioPlayer::getPitchShiftInSemiTones() const {
    return pitchShift;
}

void PortAudioPlayer::setPitchShiftInSemiTones(int value) {
    if (value == pitchShift) {
        return;
    }

    pitchShift = value;
    onTonalityChanged(value);
}

const PlaybackData &PortAudioPlayer::getPlaybackData() const {
    return playbackData;
}

void PortAudioPlayer::setupPlaybackStartedListener() {
    assert(dataSentToOutputListenerKey == 0);
    dataSentToOutputListenerKey = onDataSentToOutputListeners.addOneShotListener([=] (void*, int) {
        Executors::ExecuteOnMainThread([this] {
            playing = true;
            onPlaybackStartedListeners.executeAll();
            dataSentToOutputListenerKey = 0;
        });
    });
}

bool PortAudioPlayer::isPrepared() const {
    return stream != nullptr;
}

double PortAudioPlayer::getTempoFactor() const {
    return tempoFactor;
}

void PortAudioPlayer::setTempoFactor(double tempoFactor) {
    this->tempoFactor = tempoFactor;
}

double PortAudioPlayer::getCallbackBufferDurationInSeconds() const {
    return AudioUtils::GetSampleTimeInSeconds(playbackData.framesPerBuffer, playbackData.sampleRate);
}

bool PortAudioPlayer::isLooping() const {
    return looping;
}

void PortAudioPlayer::setLooping(bool looping) {
    this->looping = looping;
}

void PortAudioPlayer::setTotalDurationInSeconds(double totalDurationInSeconds) {
    playbackData.totalDurationInSeconds = totalDurationInSeconds;
}

void PortAudioPlayer::setPlaybackData(const PlaybackData &playbackData) {
    this->playbackData = playbackData;
}

bool PortAudioPlayer::isCompleted() const {
    return completed;
}

const std::string &PortAudioPlayer::getPlayerName() const {
    return playerName;
}

void PortAudioPlayer::setPlayerName(const std::string &playerName) {
    this->playerName = playerName;
}

void PortAudioPlayer::initSoundTouch() {
    assert(!soundTouch && "SoundTouch already initialised");
    soundTouch = new soundtouch::SoundTouch();
}

void PortAudioPlayer::onTonalityChanged(int value) {
    assert(pitchShift == 0 || soundTouch && "tonality changes are not allowed, soundtouch not "
                                            "initialised, call initSoundTouch() before prepare to use pitch shifting");
    soundTouch->setPitchSemiTones(value);
}

int PortAudioPlayer::readAudioDataApplySoundTouchIfNeed(void *outputBuffer, int requestedFramesCount) {
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
