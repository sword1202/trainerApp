//
// Created by Ivan Kuskov on 3/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef MIDITRACK_H
#define MIDITRACK_H

#include "MidiNote.h"
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <unordered_map>

struct MidiTrack
{
    std::string           instrumentName;
    std::string           trackName;
    int                   polyphonicTracksCount = 1;
    int                   channelId;
    int                   trackId;
    int                   noteCount;
    int                   lowestNote;
    int                   highestNote;
    double                averageVelocity;
    double                maxNoteValuePercent;
    double                durationInTime;

    int                   startTick = -1;
    int                   finalTick = -1;

    MidiTrack();

private:
    using MidiNotePointersVector = std::vector<std::shared_ptr<MidiNote> >;
    using MidiNoteDurationDistib = std::unordered_map <int, int>;
    MidiNotePointersVector notes;
    MidiNoteDurationDistib durationDistrib;

    std::list<int> pendingNoteIndexes;

public:
    void reset();
	std::vector<std::shared_ptr<MidiNote> > &getNotes();
    void openNote(const int keyNumber, const int tick, const int velocity);
    std::list<int>::iterator closeNote(const int keyNumber, const int tick);
    void closeAllNotes(const int tick);

    void postProcess(const int tpq, const int lastTick);
    int durationInTicks();
};

#endif // MIDITRACK_H
