//
// Created by Semyon Tikhonenko on 7/6/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PIANODRAWER_H
#define VOCALTRAINER_PIANODRAWER_H


#include "Drawer.h"
#include "Pitch.h"
#include "PlayingPitchSequence.h"
#include "PianoController.h"

class PianoDrawer : public PianoController {
    float intervalHeight = 0;
    Drawer* drawer;
    PlayingPitchSequence* pitchSequence;

    Pitch firstPitch;
    Pitch detectedPitch;
    std::vector<Drawer::Color> drawSharpPitchesFillColor;
    std::vector<float> drawSharpPitchesY;
    std::vector<int> selectedWhitePitchIndexes;

    float getIntervalOctaveHeightToPianoOctaveHeightRelation() const;
public:
    PianoDrawer(Drawer *drawer, PlayingPitchSequence *pitchSequence);
    void draw(float pianoWidth, float height, float devicePixelRation);
    void setIntervalHeight(float intervalHeight);
    void setFirstPitch(const Pitch &firstPitch);
    void setDetectedPitch(const Pitch &detectedPitch);
};


#endif //VOCALTRAINER_PIANODRAWER_H
