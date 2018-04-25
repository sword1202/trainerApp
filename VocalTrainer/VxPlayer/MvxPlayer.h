//
// Created by Semyon Tikhonenko on 1/15/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_MVXPLAYER_H
#define VOCALTRAINER_MVXPLAYER_H

#include <iostream>
#include "VxFile.h"

class MvxPlayer {
    std::string audioData;
    std::vector<char> vxAudioData;
    AudioPlayer instrumentalPlayer;
    AudioPlayer vxPlayer;
public:
    MvxPlayer();
    void loadFromFile(const char* filePath);
    void loadFromStream(std::istream& is);
    void initFromInstrumentalFileAndVxFile(const char* instrumentalFilePath, const VxFile& vxFile);
    void initFromInstrumentalStreamAndVxFile(std::istream& instrumentalStream, const VxFile& vxFile);
    void play(float instrumentalVolume, float pianoVolume);
    void setInstrumentalVolume(float instrumentalVolume);
    void setPianoVolume(float pianoVolume);
    void pause();
    void resume();
    void seek(double value);
};


#endif //VOCALTRAINER_MVXPLAYER_H
