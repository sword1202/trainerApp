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

struct VxPitch {
    Pitch pitch;
    int startBitNumber;
    int bitsCount;
};


#endif //VOCALTRAINER_VXPITCHDEFENITION_H
