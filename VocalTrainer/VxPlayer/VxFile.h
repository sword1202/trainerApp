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
#include "StlDebugUtils.h"
#include <iostream>

class VxFile {
    std::vector<VxPitch> pitches;
    int ticksPerSecond = 0;
    int durationInTicks = 0;
    int distanceInTicksBetweenLastPitchEndAndTrackEnd = 0;
    int lowestPitchIndex;
    int highestPitchIndex;

    boost::container::static_vector<VxLyricsLine, 2> lyrics;

    bool validatePitches();
    bool validateLyrics();
    void postInit();

    friend class boost::serialization::access;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const {
        ar & ticksPerSecond;
        ar & pitches;
        ar & distanceInTicksBetweenLastPitchEndAndTrackEnd;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version) {
        ar & ticksPerSecond;
        ar & pitches;
        ar & distanceInTicksBetweenLastPitchEndAndTrackEnd;
        postInit();
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
    VxFile();
    VxFile(std::vector<VxPitch> &&pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, int ticksPerSecond);
    VxFile(const std::vector<VxPitch> &pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, int ticksPerSecond);
    VxFile(VxFile&& vxFile) = default;
    VxFile(const VxFile& vxFile) = default;
    VxFile& operator=(const VxFile& vxFile) = default;

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

    void shift(int distance);
    VxFile shifted(int distance);
    bool canBeShifted(int distance) const;

    void removeSilenceSpaceFromBeginning();

    //iteratePitchesInTimeRange begin timeBegin = 47.619 timeEnd = 62.3083
    template<typename Function>
    void iteratePitchesInTickRange(int startTick, int endTick, const Function& function) const {
        for (const auto& pitch : pitches) {
            if (pitch.intersectsWith(startTick, endTick)) {
                function(pitch);
            }
        }
    }

    // startTick = 15165 endTick = 21041
    template<typename Function>
    void iteratePitchesInTimeRange(double startTime, double endTime, const Function& function) const {
        int startTick = timeInSecondsToTicks(startTime);
        int endTick = timeInSecondsToTicks(endTime);
        std::cout<<"startTick = "<<startTick<<" endTick = "<<endTick<<"\n";

        iteratePitchesInTickRange(startTick, endTick, function);
    }

    template<typename OutputIterator>
    void getPitchesInTimeRange(double startTime, double endTime, OutputIterator iterator) const {
        iteratePitchesInTimeRange(startTime, endTime, [&] (const VxPitch& vxPitch) {
            double pitchStartTime = ticksToSeconds(vxPitch.startTickNumber);
            double pitchDuration = ticksToSeconds(vxPitch.ticksCount);
            *iterator++ = vxPitch;
        });
    }
};

#endif //VOCALTRAINER_VXFILE_H
