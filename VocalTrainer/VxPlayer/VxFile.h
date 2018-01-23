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
#include "SynchronizedCallbacksQueue.h"

class VxFile {
    std::vector<VxPitchDefinition> pitches;
    AudioPlayer* player = 0;
    volatile bool isPlaying = false;
    volatile bool isPaused = false;
    double currentSeek;
    CppUtils::SynchronizedCallbacksQueue playerQueue;

    void initFromStream(std::istream& stream);
public:
    void load(std::istream& stream);
    void load(const char* fileName);
    VxFile();
    VxFile(const std::vector<VxPitchDefinition> &pitches);
    ~VxFile();

    void play();
    void pause();
    void stop();
    void seek(double timeStamp);
    void reset();
};


#endif //VOCALTRAINER_VXFILE_H
