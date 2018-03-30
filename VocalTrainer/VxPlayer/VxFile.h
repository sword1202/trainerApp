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
    int ticksPerMinute;
    int durationInBits = 0;
    int distanceInTicksBetweenLastPitchEndAndTrackEnd = 0;

    bool validate();
    void postInit();
public:
    VxFile(const std::vector<VxPitch> &pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, int bitsPerMinute);
    VxFile(std::istream& is);
    static VxFile fromFilePath(const char* filePath);

    std::vector<char> generateRawPcmAudioData(int sampleRate) const;
    std::vector<char> generateWavAudioData() const;

    double getDurationInSeconds() const;
    double getBitDuration() const;

    const std::vector<VxPitch> &getPitches() const;
    int getTicksPerMinute() const;
    int getDurationInTicks() const;
    int getDistanceInTicksBetweenLastPitchEndAndTrackEnd() const;

    void writeToStream(std::ostream& os) const;
};


#endif //VOCALTRAINER_VXFILE_H
