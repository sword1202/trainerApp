//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXPITCHDEFENITION_H
#define VOCALTRAINER_VXPITCHDEFENITION_H

#ifdef __APPLE__
#import "Pitch.h"
#else
#include "Pitch.h"
#endif
#include <vector>

struct VxPitchDefinition {
    Pitch pitch;
    double timestamp;
    double duration;
    std::vector<char> audioData;
    VxPitchDefinition(const Pitch &pitch, double timestamp);
    VxPitchDefinition();
};


#endif //VOCALTRAINER_VXPITCHDEFENITION_H
