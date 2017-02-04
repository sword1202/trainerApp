//
// Created by Semyon Tikhonenko on 2/5/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_SINGERPITCHDETECTION_H
#define VOCALTRAINER_SINGERPITCHDETECTION_H


#include "Pitch.h"

struct SingerPitchDetection {
    double time;
    Pitch pitch;

    SingerPitchDetection(const Pitch &pitch, double time);
};


#endif //VOCALTRAINER_SINGERPITCHDETECTION_H
