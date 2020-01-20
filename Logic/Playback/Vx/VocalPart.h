//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VOCAL_PART_H
#define VOCALTRAINER_VOCAL_PART_H

#include <istream>
#include "NoteInterval.h"
#include <vector>
#include <boost/container/static_vector.hpp>
#include <boost/serialization/vector.hpp>
#include "StlDebugUtils.h"
#include <iostream>

class VocalPart {
    std::vector<NoteInterval> pitches;
    double ticksPerSecond = 0;
    int durationInTicks = 0;
    int endSilenceDurationInTicks = 0;
    int lowestPitchIndex;
    int highestPitchIndex;

    bool validatePitches();
    void postInit();

    friend class boost::serialization::access;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const {
        ar & ticksPerSecond;
        ar & pitches;
        ar & endSilenceDurationInTicks;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version) {
        ar & ticksPerSecond;
        ar & pitches;
        ar & endSilenceDurationInTicks;
        postInit();
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
    VocalPart();
    VocalPart(std::vector<NoteInterval> &&pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, double ticksPerSecond);
    VocalPart(const std::vector<NoteInterval> &pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, double ticksPerSecond);
    VocalPart(VocalPart&& vxFile) = default;
    VocalPart(const VocalPart& vocalPart) = default;
    VocalPart& operator=(const VocalPart& vocalPart) = default;

    static VocalPart fromFilePath(const char* filePath);
    VocalPart(std::istream& is);
    void writeToStream(std::ostream& os) const;

    static int startTickNumberKeyProvider(const NoteInterval &pitch);

    double getDurationInSeconds() const;
    double getTickDurationInSeconds() const;
    double ticksToSeconds(int ticks) const;
    int timeInSecondsToTicks(double timeInSeconds)const;
    int samplesCountFromTicks(int ticks, int sampleRate) const;
    int ticksFromSamplesCount(int samplesCount, int sampleRate) const;

    const std::vector<NoteInterval> &getPitches() const;

    void setPitches(const std::vector<NoteInterval> &pitches);

    int getTicksPerSecond() const;
    int getDurationInTicks() const;
    int getEndSilenceDurationInTicks() const;
    const NoteInterval& getShortestPitch() const;

    void shift(int distance);
    VocalPart shifted(int distance);
    bool canBeShifted(int distance) const;

    VocalPart withChangedTempo(double tempoFactor) const;

    void removeSilenceSpaceFromBeginning();

    bool hasPitchesInMoment(double time) const;
    bool hasPitchInMoment(double time, const Pitch& pitch) const;

    double getFirstPitchStartTime() const;

    int getLowestPitchIndex() const;
    int getHighestPitchIndex() const;

    const NoteInterval& getLowestVxPitch() const;
    const NoteInterval& getHighestVxPitch() const;

    const Pitch& getLowestPitch() const;
    const Pitch& getHighestPitch() const;

    VocalPart cutOrExpand(double start, double end);

    bool isEmpty() const;

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
            const NoteInterval& pitch = pitches[i];
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
        iteratePitchesInTimeRange(startTime, endTime, [&] (const NoteInterval& vxPitch) {
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

#endif //VOCALTRAINER_VOCAL_PART_H
