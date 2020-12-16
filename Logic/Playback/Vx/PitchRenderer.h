//
// Created by Semyon Tikhonenko on 12/13/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_PITCHRENDERER_H
#define VOCALTRAINER_PITCHRENDERER_H

#include "Pitch.h"

class PitchRenderer {
public:
    virtual ~PitchRenderer() = default;
    virtual void init(int sampleRate, int numberOfChannels, int maxFramesPerBuffer) = 0;
    virtual void on(const Pitch& pitch) = 0;
    virtual void off(const Pitch& pitch) = 0;
    virtual void render(int16_t* outBuffer, int framesCount) = 0;
};


#endif //VOCALTRAINER_PITCHRENDERER_H
