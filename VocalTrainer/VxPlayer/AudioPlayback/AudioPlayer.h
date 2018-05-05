//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOPLAYER_H
#define VOCALTRAINER_AUDIOPLAYER_H

#include <portaudio/portaudio.h>
#include "ListenersSet.h"

class AudioPlayer {
protected:
    struct PlaybackData {
        int sampleRate = -1;
        int framesPerBuffer = -1;
        int numChannels = -1;
        double totalDurationInSeconds = -1;
        PaSampleFormat format = paInt16;
    };
private:
    PaStream* stream = nullptr;
    PlaybackData playbackData;
    bool playing = false;
    float volume = 1.0f;
    CppUtils::ListenersSet<> onCompleteListeners;
    CppUtils::ListenersSet<double /*seek*/, double/*totalDuration*/> seekChangedListeners;

    static int callback(const void *inputBuffer,
            void *outputBuffer,
            unsigned long framesPerBuffer,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void *userData);
protected:
    virtual int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData& playbackData) = 0;
	virtual void prepareAndProvidePlaybackData(PlaybackData* playbackData) = 0;
	virtual int getBufferSeek() const = 0;
	virtual void setBufferSeek(int bufferSeek);
	virtual double bufferSeekToSecondsSeek(int bufferSeek) const;
    virtual int secondsSeekToBufferSeek(double timestamp) const;

	virtual void onComplete();

	int secondsToSamplesCount(double secondsSeek) const;
    double samplesCountToSeconds(int samplesCount) const;
    int getSampleSize() const;

    // Audio Player can't be destroyed using delete operator, call destroy method instead;
	virtual ~AudioPlayer();
public:
    
    typedef CppUtils::ListenersSet<>::function OnCompleteListener;
    typedef CppUtils::ListenersSet<double, double>::function SeekChangedListener;
    
    AudioPlayer();
    void prepare();
    void prepareAsync(std::function<void()> callback);
    void play(double seek);
    void play();
    bool isPlaying() const;
    void pause();

	// volume - [0.0, 1.0]
    float getVolume() const;
    void setVolume(float volume);

    virtual void setSeek(double timeStamp);
    double getSeek() const;

    double getTrackDurationInSeconds();

    int addOnCompleteListener(const OnCompleteListener& listener);
    void removeOnCompleteListener(int key);

    int addSeekChangedListener(const SeekChangedListener& listener);
    void removeSeekChangedListener(int key);

    void playFromSeekToSeek(double a, double b, const std::function<void()> onFinish);
    virtual void destroy(const std::function<void()>& onDestroyed);
    void destroy();
};
#endif //VOCALTRAINER_AUDIOPLAYER_H
