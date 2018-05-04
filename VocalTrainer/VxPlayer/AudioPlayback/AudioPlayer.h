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
	virtual void setBufferSeek(int bufferSeek) = 0;

	virtual void onComplete();

	int secondsToSamplesCount(double secondsSeek) const;
    double samplesCountToSeconds(int samplesCount) const;
    int getSampleSize() const;
public:
    AudioPlayer();
    virtual ~AudioPlayer();
    void prepare();
    void play(double seek);
    void play();
    bool isPlaying() const;
    void pause();

	// volume - [0.0, 1.0]
    float getVolume() const;
    void setVolume(float volume);

    virtual void setSeek(double timeStamp);
    virtual double getSeek() const;

    double getTrackDurationInSeconds();

    int addOnCompleteListener(const CppUtils::ListenersSet<>::function& listener);
    int addOnCompleteOneShotListener(const CppUtils::ListenersSet<>::function& listener);
    void removeOnCompleteListener(int key);

    void playFromSeekToSeek(double a, double b);
};
#endif //VOCALTRAINER_AUDIOPLAYER_H
