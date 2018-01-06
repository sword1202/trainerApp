//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_WAVAUDIODATAFROMPITCHGENERATOR_H
#define VOCALTRAINER_WAVAUDIODATAFROMPITCHGENERATOR_H


#include <vector>
#include "Pitch.h"

class tsf;

class WavAudioDataFromPitchGenerator {
    tsf* t;
public:
    WavAudioDataFromPitchGenerator();
    ~WavAudioDataFromPitchGenerator();
    std::vector<char> pitchToWavAudioData(const Pitch& pitch, double duration);
};


#endif //VOCALTRAINER_WAVAUDIODATAFROMPITCHGENERATOR_H
