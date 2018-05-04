//
// Created by Semyon Tikhonenko on 1/15/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_MVXPLAYER_H
#define VOCALTRAINER_MVXPLAYER_H

#include <iostream>
#include "VxFile.h"

class MvxPlayer {
    const VxFile* vxFile;
    AudioPlayer* instrumentalPlayer;
    AudioPlayer* vxPlayer;
    bool destroyVxFileOnDestructor = false;

    void init(std::istream& is);
    void init(std::istream& instrumentalStream, const VxFile* vxFile);
public:
    ~MvxPlayer();
    MvxPlayer(const char* filePath);
    MvxPlayer(std::istream& is);
    MvxPlayer(const char* instrumentalFilePath, const VxFile* vxFile);
    MvxPlayer(std::istream& instrumentalStream, const VxFile* vxFile);
    void prepare();
    void play(float instrumentalVolume, float pianoVolume);
    void setInstrumentalVolume(float instrumentalVolume);
    void setPianoVolume(float pianoVolume);
    void pause();
    void resume();
    void seek(double value);
};


#endif //VOCALTRAINER_MVXPLAYER_H
