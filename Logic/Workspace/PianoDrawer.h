//
// Created by Semyon Tikhonenko on 7/6/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PIANODRAWER_H
#define VOCALTRAINER_PIANODRAWER_H


#include "Drawer.h"
#include "Pitch.h"
#include "PlayingPitchSequence.h"
#include "WorkspaceColorScheme.h"
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
    std::unordered_set<int> selectedPitchIndexes;
    int fontSize = 8;
    Drawer::FontStyle fontStyle;
    const WorkspaceColorScheme* colors;

    float getIntervalOctaveHeightToPianoOctaveHeightRelation() const;

    Pitch getFirstPitch() const;
public:
    void setFontSize(int fontSize);
    void setFontStyle(Drawer::FontStyle fontStyle);

    static const Drawer::Color PITCH_TEXT_COLOR;
    static const Drawer::Color SELECTED_PITCH_TEXT_COLOR;

    PianoDrawer(Drawer *drawer, const WorkspaceColorScheme* colors);

    void setPitchSequence(PlayingPitchSequence *pitchSequence);
    void draw(float pianoWidth, float height, float devicePixelRation);
    void setIntervalHeight(float intervalHeight);
    void setFirstPitch(const Pitch &firstPitch);
    void setDetectedPitch(const Pitch &detectedPitch);
    void drawPitchNames(float height) const;
    void drawSharpPitches() const;
    int getFontSize() const;
};


#endif //VOCALTRAINER_PIANODRAWER_H
