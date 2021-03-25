//
// Created by Semyon Tikhonenko on 5/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PITCHESCOLLECTOR_H
#define VOCALTRAINER_PITCHESCOLLECTOR_H

#include "Pitch.h"

class PitchesCollection {
public:
    virtual void getPitchesInTimeRange(double begin, double end,
            std::vector<double>* timesOut,
            std::vector<float>* frequenciesOut) const = 0;
    virtual Pitch getNearestPitch(double time) const = 0;
    virtual std::vector<double> getTimes() const = 0;
    virtual std::vector<float> getFrequencies() const = 0;
    virtual ~PitchesCollection() = default;
};


#endif //VOCALTRAINER_PITCHESCOLLECTOR_H
