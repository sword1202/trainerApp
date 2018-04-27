//
// Created by Semyon Tikhonenko on 4/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H
#define VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H


#include "VxFile.h"
#include "tsf.h"

struct VxFileAudioDataGeneratorConfig {
    int sampleRate = 44100;
    int outBufferSize = 2048;
};

class VxFileAudioDataGenerator {
    const VxFile* vxFile;
    std::vector<short> pcmData;
    std::vector<char> initializedPcmDataFlags; // std::vector<bool> is bad use std::vector<char>
    int outBufferSize;
    int seek = 0;
    int renderedDataSize = 0;
    int renderedPitchesCount = 0;
    int sampleRate;
    double bufferLengthInSeconds;
    tsf* _tsf;
public:
    VxFileAudioDataGenerator(const VxFile* vxFile, const VxFileAudioDataGeneratorConfig& config);
    VxFileAudioDataGenerator(const VxFile* vxFile);
    ~VxFileAudioDataGenerator();
    void reset();
    void renderNextPitch();
    bool canRenderNextPitch() const;
    int readNextSamplesBatch(short* intoBuffer);
};


#endif //VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H
