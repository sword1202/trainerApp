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
#include "VxLyricsLine.h"
#include "PlaybackChunk.h"
#include <boost/container/static_vector.hpp>
#include <boost/serialization/vector.hpp>

class VxFile {
    std::vector<VxPitch> pitches;
    int ticksPerSecond = 0;
    int durationInTicks = 0;
    int distanceInTicksBetweenLastPitchEndAndTrackEnd = 0;

    boost::container::static_vector<VxLyricsLine, 2> lyrics;

    bool validatePitches();
    bool validateLyrics();
    void postInit();

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & ticksPerSecond;
        ar & pitches;
        ar & distanceInTicksBetweenLastPitchEndAndTrackEnd;

        ar & boost::serialization::make_array(lyrics.data(), lyrics.size());
    }

public:
    VxFile();
    VxFile(std::vector<VxPitch> &&pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, int ticksPerSecond);
    VxFile(const std::vector<VxPitch> &pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, int ticksPerSecond);
    VxFile(VxFile&& vxFile) = default;
    VxFile(const VxFile& vxFile) = default;

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

    void setPitches(const std::vector<VxPitch> &pitches);

    int getTicksPerSecond() const;
    int getDurationInTicks() const;
    int getDistanceInTicksBetweenLastPitchEndAndTrackEnd() const;
    const VxPitch& getShortestPitch() const;

    const VxLyricsLine &getLyricsLine(int lineIndex) const;
    int getLyricsLinesCount() const;
    void addLyricsLine(const VxLyricsLine& line);
    void writeToStream(std::ostream& os) const;
};

#endif //VOCALTRAINER_VXFILE_H
