//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOPLAYER_H
#define VOCALTRAINER_AUDIOPLAYER_H

#ifndef __APPLE__

#include <windows.h>

#endif 

class AudioPlayer {
public:
    ~AudioPlayer();
    void play(const char* audioData, int size, double seek);
    bool isPlaying();
    void stop();
    void pause();
    void resume();
	// volume - [0.0, 1.0]
	void setVolume(float volume);
    void seek(double timeStamp);
	double getTrackDurationInSeconds();
};
#endif //VOCALTRAINER_AUDIOPLAYER_H
