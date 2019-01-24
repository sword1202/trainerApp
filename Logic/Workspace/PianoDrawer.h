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
#include <atomic>

class PianoDrawer {
    std::atomic<float> intervalHeight;
    Drawer* drawer;
    PlayingPitchSequence* pitchSequence = nullptr;

    std::atomic_int firstPitchIndex;
    std::atomic_int detectedPitchIndex;
    std::vector<Drawer::Color> drawSharpPitchesFillColor;
    std::vector<float> drawSharpPitchesY;
    std::unordered_set<int> selectedWhitePitchIndexes;

    float getIntervalOctaveHeightToPianoOctaveHeightRelation() const;

    Pitch getFirstPitch() const;
public:
    static constexpr int FONT_SIZE = 8;
    static const Drawer::Color PITCH_TEXT_COLOR;
    static const Drawer::Color SELECTED_PITCH_TEXT_COLOR;

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
