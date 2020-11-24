//
// Created by Semyon Tykhonenko on 1/21/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_PITCHSOUNDPACK_H
#define VOCALTRAINER_PITCHSOUNDPACK_H

#include <map>
#include "AudioData.h"

class PitchSoundPack {
    std::map<int, AudioData> frequencySoundMap;
public:
    PitchSoundPack(const char* packFilePath);
};


#endif //VOCALTRAINER_PITCHSOUNDPACK_H
