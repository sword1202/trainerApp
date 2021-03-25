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

    bool requestOffPitches;
    std::vector<int> pitchesIndexes;
    std::vector<int> tempPitchIndexes;
    std::vector<int> difference;

    PlaybackData playbackData;
    PitchRenderer* pitchRenderer;

    template <typename Callback>
    void iteratePitches(const std::vector<int>& indexes, const Callback& callback) {
        for (int pitchIndex : indexes) {
            const Pitch& pitch = vocalPart.getNotes()[pitchIndex].pitch;
            callback(pitch);
        }
    }

    void onPitches(const std::vector<int>& indexes);
    void offPitches(const std::vector<int>& indexes);

    void prepareForVocalPartSet(const VocalPart& vocalPart);
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

    void setVocalPart(VocalPart &&vocalPart);
    void setVocalPart(const VocalPart &vocalPart);
};


#endif //VOCALTRAINER_VXFILEAUDIODATAGENERATOR_H
