//
// Created by Semyon Tikhonenko on 7/20/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H
#define VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H

#include <vector>
#include <mutex>

#include "VocalPart.h"
#include "PlaybackData.h"
#include "PitchRenderer.h"

class VocalPartAudioDataGenerator {
    VocalPart vocalPart;
    mutable std::mutex vxFileMutex;

    int seek = 0;
    mutable std::mutex seekMutex;
    int pcmDataSamplesCount;

    std::vector<int> pitchesIndexes;
    std::vector<int> tempPitchIndexes;
    std::vector<int> difference;

    PlaybackData playbackData;
    PitchRenderer* pitchRenderer;
public:
    VocalPartAudioDataGenerator(PitchRenderer* pitchRenderer);
    void init(const PlaybackData &config);
    ~VocalPartAudioDataGenerator();

    int readNextSamplesBatch(short *intoBuffer, bool moveSeekAndFillWithZero = false);
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
