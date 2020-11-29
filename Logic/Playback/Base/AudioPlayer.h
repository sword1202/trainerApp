//
// Created by Semyon Tikhonenko on 11/28/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOPLAYER_H
#define VOCALTRAINER_AUDIOPLAYER_H


#include "ListenersSet.h"

class AudioPlayer {
public:
    CppUtils::ListenersSet<> onCompleteListeners;
    CppUtils::ListenersSet<> onNoDataAvailableListeners;
    CppUtils::ListenersSet<> onPlaybackStartedListeners;
    CppUtils::ListenersSet<> onPlaybackStoppedListeners;
    CppUtils::SynchronizedListenersSet<void*, int> onDataSentToOutputListeners; // <buffer, framesCount>
    CppUtils::SynchronizedListenersSet<double, double> seekChangedListeners; // <seek, totalDuration>

    virtual void prepare() = 0;
    virtual void prepareAsync(const std::function<void()>& callback);
    virtual void play(double seek) = 0;
    virtual void play();
    virtual bool isPlaying() const = 0;
    virtual bool isPrepared() const = 0;
    virtual void pause() = 0;
    // pause and wait for operation to be finished on the calling thread
    virtual void pauseSync() = 0;

    // volume - [0.0, 1.0]
    virtual float getVolume() const = 0;
    virtual void setVolume(float volume) = 0;

    virtual void setSeek(double timeStamp) = 0;
    virtual double getSeek() const = 0;

    virtual int getPitchShiftInSemiTones() const = 0;
    virtual void setPitchShiftInSemiTones(int value) = 0;

    virtual double getTempoFactor() const = 0;
    virtual void setTempoFactor(double tempoFactor) = 0;

    virtual double getTrackDurationInSeconds() const = 0;

    virtual double getCallbackBufferDurationInSeconds() const = 0;

    virtual bool isLooping() const = 0;
    virtual void setLooping(bool looping) = 0;

    virtual bool isCompleted() const = 0;

    virtual const std::string &getPlayerName() const = 0;
};


#endif //VOCALTRAINER_AUDIOPLAYER_H
