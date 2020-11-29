//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PORTAUDIOPLAYER_H
#define VOCALTRAINER_PORTAUDIOPLAYER_H

#include "AudioPlayer.h"
#include <portaudio/portaudio.h>
#include <mutex>
#include "ListenersSet.h"
#include "atomic"
#include "PlaybackData.h"
#include <SoundTouch/SoundTouch.h>

class PortAudioPlayer : public AudioPlayer {
private:
    PaStream* stream = nullptr;
    PlaybackData playbackData;
    std::atomic_bool playing;
    std::atomic<float> volume;
    std::atomic_bool completed;
    // used for debug purposes
    std::string playerName;

    std::atomic_int pitchShift;
    double tempoFactor = 1;
    int dataSentToOutputListenerKey = 0;
    std::atomic_bool looping;

    soundtouch::SoundTouch* soundTouch = nullptr;
    std::vector<float> soundTouchTempFloatBuffer;

    void setupPlaybackStartedListener();
	void pauseStream();

    static int callback(const void *inputBuffer,
            void *outputBuffer,
            unsigned long framesPerBuffer,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void *userData);

    int readAudioDataApplySoundTouchIfNeed(void *outputBuffer, int requestedFramesCount);

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

	int secondsToSamplesCount(double secondsSeek) const;
    double samplesCountToSeconds(int samplesCount) const;
    int getSampleSize() const;

    virtual void destroy();

    // Used for players, where totalDurationInSeconds can be changed after prepare
    void setTotalDurationInSeconds(double totalDurationInSeconds);

	void initSoundTouch();
	void setPlayerName(const std::string &playerName);
    const PlaybackData &getPlaybackData() const;
public:
    
    PortAudioPlayer();
    virtual ~PortAudioPlayer();
    void prepare();
    void play(double seek);
    bool isPlaying() const;
    bool isPrepared() const;
    void pause();
    // pause and wait for operation to be finished on the calling thread
    void pauseSync();

	// volume - [0.0, 1.0]
    float getVolume() const;
    void setVolume(float volume);

    virtual void setSeek(double timeStamp);
    double getSeek() const;

    virtual int getPitchShiftInSemiTones() const;
    void setPitchShiftInSemiTones(int value);

    virtual double getTempoFactor() const;
    virtual void setTempoFactor(double tempoFactor);

    double getTrackDurationInSeconds() const;

	double getCallbackBufferDurationInSeconds() const;

    bool isLooping() const;
    void setLooping(bool looping);

	bool isCompleted() const;

	const std::string &getPlayerName() const;
};
#endif //VOCALTRAINER_PORTAUDIOPLAYER_H
