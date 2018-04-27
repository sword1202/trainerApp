//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PITCHRENDERER_H
#define VOCALTRAINER_PITCHRENDERER_H


#include "Pitch.h"

class PitchRenderer {
    int sampleRate;
public:
    PitchRenderer(int sampleRate);
    virtual void render(const Pitch& pitch, short* buffer, int length) = 0;
    virtual ~PitchRenderer();

    int getSampleRate() const;
};


#endif //VOCALTRAINER_PITCHRENDERER_H
