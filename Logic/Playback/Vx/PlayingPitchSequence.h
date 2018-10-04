//
// Created by Semyon Tikhonenko on 7/6/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PLAYINGPITCHSEQUENCE_H
#define VOCALTRAINER_PLAYINGPITCHSEQUENCE_H


#include "Pitch.h"

class PlayingPitchSequence {
public:
    virtual bool hasPitchNow(const Pitch &pitch) const = 0;
    virtual bool hasAnyPitchNow() const = 0;
};


#endif //VOCALTRAINER_PLAYINGPITCHSEQUENCE_H
