//
// Created by Semyon Tikhonenko on 7/20/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H
#define VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H

#include <vector>
#include <mutex>

#include "VocalPartAudioDataGeneratorConfig.h"
#include "VocalPart.h"
#include "tsf.h"

class VocalPartAudioDataGenerator {
    VocalPart vocalPart;
    mutable std::mutex vxFileMutex;

    int outBufferSize;
    int seek = 0;
    mutable std::mutex seekMutex;
    int sampleRate;

    std::vector<int> pitchesIndexes;
    std::vector<int> tempPitchIndexes;
    std::vector<int> difference;

    int pcmDataSize = 0;

    tsf* _tsf;
public:
    VocalPartAudioDataGenerator();
    VocalPartAudioDataGenerator(const VocalPart& vocalPart, const VocalPartAudioDataGeneratorConfig &config);
    VocalPartAudioDataGenerator(const VocalPart& vocalPart);
    ~VocalPartAudioDataGenerator();

    int readNextSamplesBatch(short *intoBuffer, bool moveSeekAndFillWithZero = false);
    std::vector<short> readAll();
    int getSeek() const;
    void setSeek(int seek);

    int getOutBufferSize() const;
    int getSampleRate() const;
    double getDurationInSeconds() const;

    const VocalPart &getVocalPart() const;

    void setVocalPart(const VocalPart &vocalPart);
    // setVocalPart and set seek to 0
    void resetVocalPart(const VocalPart &vocalPart);
};


#endif //VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H
