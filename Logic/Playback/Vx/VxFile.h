//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXFILE_H
#define VOCALTRAINER_VXFILE_H

#include <istream>
#include "VxPitch.h"
#include <vector>
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

    bool validatePitches();
    void postInit();

#ifdef USE_BOOST_SERIALIZATION

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

#endif

public:
    VxFile();
    VxFile(std::vector<VxPitch> &&pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, int ticksPerSecond);
    VxFile(const std::vector<VxPitch> &pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, int ticksPerSecond);
    VxFile(VxFile&& vxFile) = default;
    VxFile(const VxFile& vxFile) = default;
    VxFile& operator=(const VxFile& vxFile) = default;

#ifdef USE_BOOST_SERIALIZATION
    static VxFile fromFilePath(const char* filePath);
    VxFile(std::istream& is);
    void writeToStream(std::ostream& os) const;
#endif

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

    void shift(int distance);
    VxFile shifted(int distance);
    bool canBeShifted(int distance) const;

    VxFile withChangedTempo(double tempoFactor) const;

    void removeSilenceSpaceFromBeginning();

    bool hasPitchesInMoment(double time) const;
    bool hasPitchInMoment(double time, const Pitch& pitch) const;

    double getFirstPitchStartTime() const;

    int getLowestPitchIndex() const;
    int getHighestPitchIndex() const;

    const VxPitch& getLowestVxPitch() const;
    const VxPitch& getHighestVxPitch() const;

    const Pitch& getLowestPitch() const;
    const Pitch& getHighestPitch() const;

    VxFile cut(double start, double end);

    template<typename Function>
    void iteratePitchesInTickRange(int startTick, int endTick, const Function& function) const {
        for (const auto& pitch : pitches) {
            if (pitch.intersectsWith(startTick, endTick)) {
                function(pitch);
            }
        }
    }

    template<typename Function>
    void iteratePitchesIndexesInTickRange(int startTick, int endTick, const Function& function) const {
        for (int i = 0; i < pitches.size(); ++i) {
            const VxPitch& pitch = pitches[i];
            if (pitch.intersectsWith(startTick, endTick)) {
                function(i);
            }
        }
    }

    template<typename Function>
    void iteratePitchesIndexesInTimeRange(double startTime, double endTime, const Function& function) const {
        int startTick = timeInSecondsToTicks(startTime);
        int endTick = timeInSecondsToTicks(endTime);
        iteratePitchesIndexesInTickRange(startTick, endTick, function);
    }

    template<typename Function>
    void iteratePitchesInTimeRange(double startTime, double endTime, const Function& function) const {
        int startTick = timeInSecondsToTicks(startTime);
        int endTick = timeInSecondsToTicks(endTime);
        iteratePitchesInTickRange(startTick, endTick, function);
    }

    template<typename OutputIterator>
    void getPitchesInTimeRange(double startTime, double endTime, OutputIterator iterator) const {
        iteratePitchesInTimeRange(startTime, endTime, [&] (const VxPitch& vxPitch) {
            *iterator++ = vxPitch;
        });
    }

    template<typename OutputIterator>
    void getPitchesIndexesInTimeRange(double startTime, double endTime, OutputIterator iterator) const {
        iteratePitchesIndexesInTimeRange(startTime, endTime, [&] (int index) {
            *iterator++ = index;
        });
    }
};

#endif //VOCALTRAINER_VXFILE_H
