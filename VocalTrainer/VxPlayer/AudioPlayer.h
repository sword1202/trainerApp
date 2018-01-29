//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOPLAYER_H
#define VOCALTRAINER_AUDIOPLAYER_H

#ifndef __APPLE__

#include <windows.h>

struct PlayingData {
	HWAVEOUT     queue;
	WAVEHDR		header;
	uint32_t bufferSizeInBytes = 0;
	char* _pBuffer = nullptr;
	double currentTime = 0;
	WAVEFORMATEX format;
};

#endif 

class AudioPlayer {

#ifdef __APPLE__
    
	void* player = 0;

#else
	PlayingData playingData;
	MMRESULT _status;
	bool bPlaying;

private:
	MMRESULT setupHeaderData();
	void initAudioDevice(const char* audioData, int size);

#endif 

public:
    ~AudioPlayer();
    void play(const char* audioData, int size, double seek);
    bool isPlaying();
    void stop();
    void pause();
    void resume();
    void seek(double timeStamp);
};
#endif //VOCALTRAINER_AUDIOPLAYER_H
