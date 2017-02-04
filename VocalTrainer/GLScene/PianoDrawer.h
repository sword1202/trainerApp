//
// Created by Semyon Tikhonenko on 2/4/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PIANODRAWER_H
#define VOCALTRAINER_PIANODRAWER_H


#include "Pitch.h"

class PianoDrawer {
public:
    void draw(const Pitch& pitch);
private:
    enum SelectorDrawType {
        WHITE, BLACK, NONE
    };

    float selectedPitchX1 = -1.0f;
    float selectedPitchX2, selectedPitchY1, selectedPitchY2;
    SelectorDrawType selectorDrawType = NONE;

    void setupPitchSelectorDrawing(const Pitch& pitch);
    void drawPitchSelector();
};


#endif //VOCALTRAINER_PIANODRAWER_H
