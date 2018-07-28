//
// Created by Semyon Tikhonenko on 7/6/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PIANODRAWER_H
#define VOCALTRAINER_PIANODRAWER_H


#include "Drawer.h"
#include "Pitch.h"
#include "PlayingPitchSequence.h"
#include <unordered_set>
#include <boost/thread/shared_mutex.hpp>

class PianoDrawer {
    std::atomic<float> intervalHeight;
    Drawer* drawer;
    PlayingPitchSequence* pitchSequence = nullptr;

    Pitch firstPitch;
    Pitch detectedPitch;
    mutable boost::shared_mutex firstPitchMutex;
    mutable boost::shared_mutex detectedPitchMutex;
    std::vector<Drawer::Color> drawSharpPitchesFillColor;
    std::vector<float> drawSharpPitchesY;
    std::unordered_set<int> selectedWhitePitchIndexes;

    float getIntervalOctaveHeightToPianoOctaveHeightRelation() const;

public:
    PianoDrawer(Drawer *drawer);

    void setPitchSequence(PlayingPitchSequence *pitchSequence);
    void draw(float pianoWidth, float height, float devicePixelRation);
    void setIntervalHeight(float intervalHeight);
    void setFirstPitch(const Pitch &firstPitch);
    void setDetectedPitch(const Pitch &detectedPitch);
    void drawPitchNames(float height) const;
    void drawSharpPitches() const;
};


#endif //VOCALTRAINER_PIANODRAWER_H
