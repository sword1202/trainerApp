//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOPLAYER_H
#define VOCALTRAINER_AUDIOPLAYER_H

#include <portaudio/portaudio.h>
#include <mutex>
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
    std::atomic_bool playing;
    std::atomic<float> volume;
    CppUtils::ListenersSet<> onCompleteListeners;
	CppUtils::ListenersSet<> onNoDataAvailableListeners;
    CppUtils::ListenersSet<> onPlaybackStartedListeners;
    CppUtils::ListenersSet<> onPlaybackStoppedListeners;
	CppUtils::ListenersSet<void*/*buffer*/, int/*framesCount*/> onDataSentToOutputListeners;
    CppUtils::ListenersSet<double /*seek*/, double/*totalDuration*/> seekChangedListeners;
    std::atomic_int pitchShift;
    double tempoFactor;
    int dataSentToOutputListenerKey = 0;

    void setupPlaybackStartedListener();

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

    virtual void destroy();
public:

    typedef CppUtils::ListenersSet<>::Listener OnCompleteListener;
	typedef CppUtils::ListenersSet<>::Listener OnNoDataAvailableListener;
    typedef CppUtils::ListenersSet<>::Listener OnPlaybackStartedListener;
    typedef CppUtils::ListenersSet<>::Listener OnPlaybackStoppedListener;
	typedef CppUtils::ListenersSet<void*, int>::Listener OnDataSentToOutputListener;
    typedef CppUtils::ListenersSet<double, double>::Listener SeekChangedListener;
    
    AudioPlayer();
    virtual ~AudioPlayer();
    void prepare();
    void prepareAsync(const std::function<void()>& callback);
    void play(double seek);
    void play();
    bool isPlaying() const;
    bool isPrepared() const;
    void pause();

	// volume - [0.0, 1.0]
    float getVolume() const;
    void setVolume(float volume);

    virtual void setSeek(double timeStamp);
    double getSeek() const;

    virtual int getPitchShiftInSemiTones() const;
    virtual void setPitchShiftInSemiTones(int value);

    virtual double getTempoFactor() const;
    virtual void setTempoFactor(double tempoFactor);

    double getTrackDurationInSeconds() const;

    int addOnCompleteListener(const OnCompleteListener& listener);
    void removeOnCompleteListener(int key);

    int addSeekChangedListener(const SeekChangedListener& listener);
    void removeSeekChangedListener(int key);

	int addOnNoDataAvailableListener(const OnNoDataAvailableListener& listener);
	void removeOnNoDataAvailableListener(int key);

	int addOnDataSentToOutputListener(const OnDataSentToOutputListener& listener);
	void removeOnDataSentToOutputListener(int key);

    int addPlaybackStartedListener(const OnPlaybackStartedListener &listener);
    void removePlaybackStartedListener(int key);

    int addPlaybackStoppedListener(const OnPlaybackStoppedListener &listener);
    void removePlaybackStoppedListener(int key);

    void playFromSeekToSeek(double a, double b, const std::function<void()> onFinish);

	const PlaybackData &getPlaybackData() const;
	double getCallbackBufferDurationInSeconds() const;
};
#endif //VOCALTRAINER_AUDIOPLAYER_H
