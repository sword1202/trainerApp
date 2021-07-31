//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VOCAL_PART_H
#define VOCALTRAINER_VOCAL_PART_H

#include <istream>
#include "NoteInterval.h"
#include <vector>
#include "StlDebugUtils.h"
#include <iostream>

static constexpr int VERSION = 1;

class VocalPart {
    std::vector<NoteInterval> notes;
    double ticksPerSecond = 0;
    int durationInTicks = 0;
    int endSilenceDurationInTicks = 0;
    int lowestPitchIndex;
    int highestPitchIndex;

    bool validateNotes();
    void postInit();
public:
    VocalPart();
    VocalPart(std::vector<NoteInterval> &&pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, double ticksPerSecond);
    VocalPart(const std::vector<NoteInterval> &pitches, int distanceInTicksBetweenLastPitchEndAndTrackEnd, double ticksPerSecond);
    VocalPart(VocalPart&& vxFile) = default;
    VocalPart(const VocalPart& vocalPart) = default;
    VocalPart& operator=(const VocalPart& vocalPart) = default;

    static int startTickNumberKeyProvider(const NoteInterval &pitch);

    double getDurationInSeconds() const;
    double getTickDurationInSeconds() const;
    double ticksToSeconds(int ticks) const;
    int timeInSecondsToTicks(double timeInSeconds)const;
    int samplesCountFromTicks(int ticks, int sampleRate) const;
    int ticksFromSamplesCount(int samplesCount, int sampleRate) const;

    const std::vector<NoteInterval> &getNotes() const;

    void setNotes(const std::vector<NoteInterval> &pitches);

    double getTicksPerSecond() const;
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

    const NoteInterval& getLowestNote() const;
    const NoteInterval& getHighestNote() const;

    const Pitch& getLowestPitch() const;
    const Pitch& getHighestPitch() const;

    VocalPart cutOrExpand(double start, double end);

    bool isEmpty() const;

    double getPitchDuration(int index) const;

    template<typename Function>
    void iteratePitchesInTickRange(int startTick, int endTick, const Function& function) const {
        for (const auto& pitch : notes) {
            if (pitch.intersectsWith(startTick, endTick)) {
                function(pitch);
            }
        }
    }

    template<typename Function>
    void iteratePitchesIndexesInTickRange(int startTick, int endTick, const Function& function) const {
        for (int i = 0; i < notes.size(); ++i) {
            const NoteInterval& pitch = notes[i];
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

    template <typename Archive>
    void saveOrLoad(Archive& archive, bool save) {
        int version = VERSION;
        archive(version);
        archive(ticksPerSecond);
        archive(notes);
        archive(endSilenceDurationInTicks);
        if (!save) {
            postInit();
        }
    }
};

#endif //VOCALTRAINER_VOCAL_PART_H
