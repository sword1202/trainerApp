//
// Created by Semyon Tikhonenko on 11/28/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_BASEAUDIOPLAYER_H
#define VOCALTRAINER_BASEAUDIOPLAYER_H


#include "ListenersSet.h"
#include "PlaybackData.h"
#include "AudioOutputWriter.h"
#include <SoundTouch/SoundTouch.h>
#include "Executors.h"

class BaseAudioPlayer : protected CppUtils::OnThreadExecutor {
    AudioOutputWriter* writer = nullptr;
    PlaybackData playbackData;
    std::atomic_bool playing;
    std::atomic<float> volume;
    std::atomic_bool completed;
    // used for debug purposes
    std::string playerName;

    std::atomic_int pitchShift;
    std::atomic<double> tempoFactor;
    int dataSentToOutputListenerKey = 0;
    std::atomic_bool looping;

    soundtouch::SoundTouch* soundTouch = nullptr;
    std::vector<float> soundTouchTempFloatBuffer;

    std::vector<std::function<void()>> onResetQueue;

    void setupPlaybackStartedListener();

    int readAudioDataApplySoundTouchIfNeed(void *outputBuffer, int requestedSamplesCount);

    void writerCallback(void* buffer, int samplesCount);
protected:
    virtual int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData& playbackData) = 0;

    virtual void providePlaybackData(PlaybackData *playbackData) = 0;
    // Call this method if you have a predefined PlaybackData, which should not be retrieved during prepare
    void setPlaybackData(const PlaybackData &playbackData);
    virtual int getBufferSeek() const = 0;
    virtual void setBufferSeek(int bufferSeek);
    virtual double bufferSeekToSecondsSeek(int bufferSeek) const;
    virtual int secondsSeekToBufferSeek(double timestamp) const;

    virtual void onComplete();
    virtual void onTonalityChanged(int value);
    virtual void onTempoFactorChanged(double value, double oldValue);

    int getSampleSize() const;

    // Used for players, where totalDurationInSeconds can be changed after prepare
    void setTotalDurationInSeconds(double totalDurationInSeconds);

    void initSoundTouch();
    void setPlayerName(const std::string &playerName);
    const PlaybackData &getPlaybackData() const;
public:
    CppUtils::ListenersSet<> onCompleteListeners;
    CppUtils::ListenersSet<> onNoDataAvailableListeners;
    CppUtils::ListenersSet<> onPlaybackStartedListeners;
    CppUtils::ListenersSet<> onPlaybackStoppedListeners;
    CppUtils::SynchronizedListenersSet<void*, int> onDataSentToOutputListeners; // <buffer, framesCount>
    CppUtils::SynchronizedListenersSet<double, double> seekChangedListeners; // <seek, totalDuration>

    BaseAudioPlayer();
    void prepare();
    virtual void play(double seek);
    virtual void play();
    virtual bool isPlaying() const;
    virtual bool isPrepared() const;
    virtual void pause();

    // volume - [0.0, 5.0], (1.0, 5.0] - overload
    virtual float getVolume() const;
    virtual void setVolume(float volume);

    virtual void setSeek(double timeStamp);
    virtual double getSeek() const;

    virtual int getPitchShiftInSemiTones() const;
    virtual void setPitchShiftInSemiTones(int value);

    virtual double getTempoFactor() const;
    virtual void setTempoFactor(double tempoFactor);

    virtual double getOriginalTrackDurationInSeconds() const;
    virtual double getTrackDurationInSecondsWithTempoApplied() const;

    virtual double getCallbackBufferDurationInSeconds() const;

    virtual bool isLooping() const;
    virtual void setLooping(bool looping);

    virtual bool isCompleted() const;

    virtual const std::string &getPlayerName() const;

    virtual void reset();
    virtual ~BaseAudioPlayer();
};


#endif //VOCALTRAINER_BASEAUDIOPLAYER_H
