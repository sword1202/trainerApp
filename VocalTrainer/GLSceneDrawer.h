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

private:
    void drawPitchesGraph(int64_t now);
    void drawWavPitches(int64_t now);
    void drawDividers();
    void studentPitchDetected(const Pitch &pitch);

    PitchInputReader* studentPitchInputReader;
    std::vector<SingerPitchDetection> detectedPitches;
    std::vector<PitchDetection> pitchesFromWavFile;
    int64_t pitchesLoadedTime;


    void drawPiano();
};


#endif //VOCALTRAINER_GLSCENEDRAWER_H
