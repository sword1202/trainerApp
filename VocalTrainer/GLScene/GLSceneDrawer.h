//
// Created by Semyon Tikhonenko on 1/12/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_GLSCENEDRAWER_H
#define VOCALTRAINER_GLSCENEDRAWER_H


#include <vector>
#include "Pitch.h"
#include "PitchInputReader.h"
#include "WavFilePitchesReader.h"
#include "PianoDrawer.h"
#include "PitchGraphDrawer.h"
#include <mutex>

class GLSceneDrawer {
public:
    GLSceneDrawer();
    ~GLSceneDrawer();

    void draw(int width, int height);
    void readPitchesFromWav(const char* wavFileName);

    void setMoveBetweenOctaves(bool moveBetweenOctaves);
private:
    void drawWavPitches(int64_t now);
    void drawBlackPitchesBackground();
    void studentPitchDetected(const Pitch &pitch);

    PitchInputReader* studentPitchInputReader;
    std::vector<PitchDetection> pitchesFromWavFile;
    int64_t pitchesLoadedTime;
    std::mutex pitchesMutex;
    PianoDrawer pianoDrawer;
    PitchGraphDrawer studentPitchGraphDrawer;
};


#endif //VOCALTRAINER_GLSCENEDRAWER_H
