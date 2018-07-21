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
#include "tsf.h"

class VxFileAudioDataGenerator {
    VxFile vxFile;

    int outBufferSize;
    int seek = 0;
    int sampleRate;

    std::vector<int> pitchesIndexes;
    std::vector<int> tempPitchIndexes;
    std::vector<int> difference;

    int pcmDataSize;

    tsf* _tsf;
public:
    VxFileAudioDataGenerator(const VxFile& vxFile, const VxFileAudioDataGeneratorConfig &config);
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
