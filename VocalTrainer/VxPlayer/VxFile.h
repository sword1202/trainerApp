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
#include "VxLyricsInterval.h"
#include "PlaybackChunk.h"

class VxFile {
    std::vector<VxPitch> pitches;
    int ticksPerSecond = 0;
    int durationInTicks = 0;
    int distanceInTicksBetweenLastPitchEndAndTrackEnd = 0;
    std::vector<VxLyricsInterval> lyrics;

    bool validatePitches();
    bool validateLyrics();
    void postInit();
    void processLyrics(const std::string& lyricsData);

public:
    VxFile(const std::vector<VxPitch> &pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, int ticksPerSecond);
    VxFile(std::istream& is);
    static VxFile fromFilePath(const char* filePath);
    static int startTickNumberKeyProvider(const VxPitch &pitch);

    double getDurationInSeconds() const;
    double getTickDurationInSeconds() const;
    double ticksToSeconds(int ticks) const;
    int timeInSecondsToTicks(double timeInSeconds)const;
    int samplesCountFromTicks(int ticks, int sampleRate) const;
    int ticksFromSamplesCount(int samplesCount, int sampleRate) const;

    const std::vector<VxPitch> &getPitches() const;
    int getTicksPerSecond() const;
    int getDurationInTicks() const;
    int getDistanceInTicksBetweenLastPitchEndAndTrackEnd() const;
    const VxPitch& getShortestPitch() const;

    const std::vector<VxLyricsInterval> &getLyrics() const;
    void setLyrics(const std::vector<VxLyricsInterval>& lyrics);
    void writeToStream(std::ostream& os) const;
};


#endif //VOCALTRAINER_VXFILE_H
