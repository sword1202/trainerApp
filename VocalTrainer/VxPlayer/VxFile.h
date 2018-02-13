//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXFILE_H
#define VOCALTRAINER_VXFILE_H

#include <istream>
#include "VxPitch.h"
#include "AudioPlayer.h"
#include <vector>
#include "SynchronizedCallbacksQueue.h"

class VxFile {
    std::vector<VxPitch> pitches;
    int bitsPerMinute;
    int durationInBits = 0;
    int trackEndSilenceBitsCount = 0;

    bool validate();
    void postInit();
public:
    VxFile(const std::vector<VxPitch> &pitches, int trackEndSilenceBitsCount, int bitsPerMinute);
    VxFile(std::istream& is);
    static VxFile fromFilePath(const char* filePath);

    std::vector<char> generateRawPcmAudioData(int sampleRate) const;
    std::vector<char> generateWavAudioData() const;

    double getDurationInSeconds() const;
    double getBitDuration() const;

    const std::vector<VxPitch> &getPitches() const;
    int getBitsPerMinute() const;
    int getDurationInBits() const;
    int getTrackEndSilenceBitsCount() const;

    void writeToStream(std::ostream& os) const;
};


#endif //VOCALTRAINER_VXFILE_H
