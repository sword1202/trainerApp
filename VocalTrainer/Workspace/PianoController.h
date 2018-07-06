//
// Created by Semyon Tikhonenko on 7/6/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PIANOCONTROLLER_H
#define VOCALTRAINER_PIANOCONTROLLER_H


#import "Pitch.h"

class PianoController {
public:
    virtual void setIntervalHeight(float intervalHeight) = 0;
    virtual void setFirstPitch(const Pitch &firstPitch) = 0;
    virtual void setDetectedPitch(const Pitch &detectedPitch) = 0;
};


#endif //VOCALTRAINER_PIANOCONTROLLER_H
