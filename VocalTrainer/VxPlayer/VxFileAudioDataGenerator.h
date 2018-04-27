//
// Created by Semyon Tikhonenko on 4/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H
#define VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H


#include "VxFile.h"
#include "tsf.h"
#include "PitchRenderer.h"
#include <set>

struct VxFileAudioDataGeneratorConfig {
    int sampleRate = 44100;
    int outBufferSize = 2048;
};

class VxFileAudioDataGenerator {
    enum DataFlag : char {
        NOT_INITIALIZED,
        PARTLY_INITIALIZED,
        FULLY_INITIALIZED
    };

    PitchRenderer* renderer;
    const VxFile* vxFile;
    std::vector<short> pcmData;
    std::vector<DataFlag> pcmDataStateFlags;
    std::set<int> renderedPitchesIndexes;
    int outBufferSize;
    int seek = 0;
    int sampleRate;
    double bufferLengthInSeconds;

    bool isFullyInitialized(int begin, int end) const;
    int getNextPitchToRenderIndex() const;
public:
    VxFileAudioDataGenerator(PitchRenderer *renderer, const VxFile *vxFile,
            const VxFileAudioDataGeneratorConfig &config);
    VxFileAudioDataGenerator(PitchRenderer *renderer, const VxFile *vxFile);
    VxFileAudioDataGenerator(const VxFile *vxFile, const VxFileAudioDataGeneratorConfig &config);
    VxFileAudioDataGenerator(const VxFile *vxFile);
    ~VxFileAudioDataGenerator();
    void reset();

    void renderNextPitch();
    bool canRenderNextPitch() const;
    int readNextSamplesBatch(short* intoBuffer);
};


#endif //VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H
