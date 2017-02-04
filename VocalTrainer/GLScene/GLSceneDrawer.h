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

class GLSceneDrawer {
public:
    struct SingerPitchDetection {
        double time;
        Pitch pitch;

        SingerPitchDetection(const Pitch &pitch, double time) : pitch(pitch), time(time) {
        }
    };

    GLSceneDrawer();
    ~GLSceneDrawer();

    void draw(int width, int height);
    void readPitchesFromWav(const char* wavFileName);

    const std::vector<SingerPitchDetection> &getDetectedPitches() const;

    bool getMoveBetweenOctaves() const;
    void setMoveBetweenOctaves(bool moveBetweenOctaves);
private:
    void drawPitchesGraph(int64_t now);
    void drawWavPitches(int64_t now);
    void drawDividers();
    void studentPitchDetected(const Pitch &pitch);

    PitchInputReader* studentPitchInputReader;
    std::vector<SingerPitchDetection> detectedPitches;
    std::vector<PitchDetection> pitchesFromWavFile;
    int64_t pitchesLoadedTime;
    bool moveBetweenOctaves;
    std::mutex pitchesMutex;
    PianoDrawer pianoDrawer;
};


#endif //VOCALTRAINER_GLSCENEDRAWER_H
