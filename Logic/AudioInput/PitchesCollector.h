//
// Created by Semyon Tikhonenko on 5/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PITCHESCOLLECTOR_H
#define VOCALTRAINER_PITCHESCOLLECTOR_H

#include "Pitch.h"

class PitchesCollector {
public:
    virtual ~PitchesCollector() = default;
    virtual int getPitchesCount() const = 0;
    virtual float getFrequencyAt(int index) const = 0;
    virtual Pitch getPitchAt(int index) const;
    virtual double getTimeAt(int index) const = 0;
    virtual int getPitchesCountAfterTime(double time) const = 0;
};


#endif //VOCALTRAINER_PITCHESCOLLECTOR_H
