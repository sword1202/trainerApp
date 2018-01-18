//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOPLAYER_H
#define VOCALTRAINER_AUDIOPLAYER_H

class AudioPlayer {
#ifdef __APPLE__
    void* player = 0;
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
