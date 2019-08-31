//
// Created by Semyon Tykhonenko on 8/31/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_REWINDABLE_H
#define VOCALTRAINER_REWINDABLE_H

#include "Seeker.h"

class Rewindable : public Seeker {
public:
    virtual double getDuration() const = 0;
};


#endif //VOCALTRAINER_REWINDABLE_H
