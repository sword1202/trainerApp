//
// Created by Semyon Tikhonenko on 1/15/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_MVXPLAYER_H
#define VOCALTRAINER_MVXPLAYER_H

#include <iostream>
#include "VxFile.h"
#include "MvxFile.h"

class MvxPlayer {
    AudioPlayer* instrumentalPlayer;
    AudioPlayer* vxPlayer;

    void init(std::istream& is);
public:
    ~MvxPlayer();
    MvxPlayer(const char* filePath);
    MvxPlayer(std::istream& is);
    MvxPlayer(MvxFile&& mvxFile);
    void prepare();
    void play(float instrumentalVolume, float pianoVolume);
    void setInstrumentalVolume(float instrumentalVolume);
    void setPianoVolume(float pianoVolume);
    void pause();
    void resume();
    void seek(double value);
};


#endif //VOCALTRAINER_MVXPLAYER_H
