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
#include "VxLyricsInterval.h"

class VxFile {
    std::vector<VxPitch> pitches;
    int ticksPerMinute = 0;
    int durationInTicks = 0;
    int distanceInTicksBetweenLastPitchEndAndTrackEnd = 0;
    std::vector<VxLyricsInterval> lyrics;

    bool validate();
    bool validateLyrics();
    void postInit();
    void processLyrics(const std::string& lyricsData);
public:
    VxFile(const std::vector<VxPitch> &pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, int bitsPerMinute);
    VxFile(std::istream& is);
    static VxFile fromFilePath(const char* filePath);

    // volume - [0.0, 1.0]
    std::vector<char> generateRawPcmAudioData(int sampleRate, float volume) const;
    std::vector<char> generateWavAudioData(float volume) const;

    double getDurationInSeconds() const;
    double getTickDuration() const;

    const std::vector<VxPitch> &getPitches() const;
    int getTicksPerMinute() const;
    int getDurationInTicks() const;
    int getDistanceInTicksBetweenLastPitchEndAndTrackEnd() const;

    const std::vector<VxLyricsInterval> &getLyrics() const;


    void setLyrics(const std::vector<VxLyricsInterval>& lyrics);

    void writeToStream(std::ostream& os) const;
};


#endif //VOCALTRAINER_VXFILE_H
