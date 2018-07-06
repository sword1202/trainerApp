//
// Created by Semyon Tikhonenko on 7/4/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H
#define VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H

#include "PitchInputReaderCollector.h"

class VxPitchInputReader : public PitchInputReaderCollector {
public:
    VxPitchInputReader();

    void pitchDetected(float frequency, double time) override;
};


#endif //VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H
