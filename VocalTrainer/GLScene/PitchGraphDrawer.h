//
// Created by Semyon Tikhonenko on 2/4/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PITCHGRAPHDRAWER_H
#define VOCALTRAINER_PITCHGRAPHDRAWER_H


#include <vector>
#include "Pitch.h"
#include "SingerPitchDetection.h"

class PitchGraphDrawer {
public:
    void onPitchDetected(const Pitch& pitch);
    const std::vector<SingerPitchDetection>& updatePitches(int64_t now);
    void draw(int64_t now);
private:
    std::vector<SingerPitchDetection> detectedPitches;
};


#endif //VOCALTRAINER_PITCHGRAPHDRAWER_H
