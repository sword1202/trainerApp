//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXFILE_H
#define VOCALTRAINER_VXFILE_H


#include <istream>
#include "VxPitchDefinition.h"
#include "AudioPlayer.h"
#include <vector>

class VxFile {
    std::vector<VxPitchDefinition> pitches;
    std::vector<std::vector<char>> audioData;
    AudioPlayer* player = 0;

    void initFromStream(std::istream& stream);
public:
    VxFile(std::istream& stream);
    VxFile(const char* fileName);
    VxFile(const std::vector<VxPitchDefinition> &pitches);
    ~VxFile();

    void play();
    void stop();
    void seek(double timeStamp);
    void reset();
};


#endif //VOCALTRAINER_VXFILE_H
