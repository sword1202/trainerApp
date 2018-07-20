//
// Created by Semyon Tikhonenko on 7/20/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H
#define VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H

#include <vector>

#include "VxFileAudioDataGeneratorConfig.h"
#include "VxFile.h"
#include "PitchRenderer.h"

class VxFileAudioDataGenerator {
    PitchRenderer* renderer;
    VxFile vxFile;

    int outBufferSize;
    int seek = 0;
    int sampleRate;

    std::vector<short> temp;
    std::vector<int> renderedPitchesSummary;
    std::vector<int> overlappingCountMap;
    std::vector<VxPitch> tempPitches;
    int pcmDataSize;
public:
    VxFileAudioDataGenerator(const VxFile& vxFile, PitchRenderer *renderer);
    VxFileAudioDataGenerator(const VxFile& vxFile, PitchRenderer *renderer,
            const VxFileAudioDataGeneratorConfig &config);
    VxFileAudioDataGenerator(const VxFile &file);
    ~VxFileAudioDataGenerator();

    int readNextSamplesBatch(short *intoBuffer);
    int getSeek() const;
    void setSeek(int seek);

    int getOutBufferSize() const;
    int getSampleRate() const;
    double getDurationInSeconds() const;

    const VxFile &getVxFile() const;

    void setVxFile(const VxFile &vxFile);
};


#endif //VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H
