//
// Created by Ivan Kuskov on 3/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MidiTrack.h"

#include <iostream>

// Considering 
// length of 4th      note == 1, 
// getting 
// length of 8th      note =  1/2
// length of 8th.dot  note = (1/2 + 1) / 2 = 3/4
// length of 16th     note =  1/4
// length of 16th.dot note =  (1/4 + 1/2) / 2 = 3/8
// etc.
static constexpr double    NOTE_VALUE_64TH_DOT_RELATIVE_TO_4TH = 3.0 / 32.0;
static constexpr double    NOTE_VALUE_32TH_DOT_RELATIVE_TO_4TH = 3.0 / 16.0;
static constexpr double    NOTE_VALUE_16TH_DOT_RELATIVE_TO_4TH = 3.0 / 8.0;
static constexpr double    NOTE_VALUE_8TH_DOT_RELATIVE_TO_4TH  = 3.0 / 4.0;
static constexpr double    NOTE_VALUE_4TH_DOT_RELATIVE_TO_4TH  = 3.0 / 2.0;
static constexpr double    NOTE_VALUE_2ND_DOT_RELATIVE_TO_4TH  = 3.0 / 1.0;

MidiTrack::MidiTrack() :
    startTick(-1),
    finalTick(-1),
	isPolyphonical(false)
{
}

std::vector<std::shared_ptr<MidiNote> > &MidiTrack::getNotes()
{
	return notes;
}

	/*!
 * \brief MidiTrack::openNote
 *
 * Creates new note with keyNumber, startTick = tick and velocity.
 * Adds index of note in array to pendingNoteIndexes.
 * If the same note is already in pending, closing it and opening new
 * If pending notes anyway contains some indexes, setting isPolyphonical to true
 * If start tick of track is not set, setting it
 * \param keyNumber
 * \param tick
 * \param velocity
 */
void MidiTrack::openNote(const int keyNumber, const int tick, const int velocity) {
    std::shared_ptr<MidiNote> note = std::make_shared<MidiNote>();
    note->startTick = tick;
    note->keyNumber = keyNumber;
    note->velocity = velocity;
    notes.push_back(note);

    // If the same note is already in pending, closing it and opening new
    std::list<int>::iterator it;
    for (it = pendingNoteIndexes.begin(); it != pendingNoteIndexes.end();) {
        auto pendingNote = notes.at(*it);
        if (pendingNote->keyNumber == keyNumber) {
            it = closeNote(keyNumber, tick);
        } else {
            ++it;
        }
    }
    if (pendingNoteIndexes.size() > 0) {
        isPolyphonical = true;
    }
    pendingNoteIndexes.emplace_back(notes.size() - 1);

    if (startTick == -1) {
        startTick = tick;
    }

}

/*!
 * \brief MidiTrack::closeNote
 *
 * Sets note's finalTick and removing its index from pendingNoteIndexes
 * \param keyNumber
 * \param tick
 * \return
 */
std::list<int>::iterator MidiTrack::closeNote(const int keyNumber, const int tick) {
    std::list<int>::iterator it;
    for (it = pendingNoteIndexes.begin(); it != pendingNoteIndexes.end();) {
        auto note = notes.at(*it);
        if (note->keyNumber == keyNumber) {
            note->finalTick = tick;
            it = pendingNoteIndexes.erase(it);
            return it;
        } else {
            ++it;
        }
    }
    return it;
}

/*!
 * \brief MidiTrack::closeAllNotes
 *
 * Closes all pending notes (see closeNote)
 * \param tick
 */
void MidiTrack::closeAllNotes(const int tick) {
    for (auto index : pendingNoteIndexes) {
        auto note = notes.at(index);
        closeNote(note->keyNumber, tick);
    }
}

/*!
 * \brief MidiTrack::postProcess
 *
 * Counts necessary values for current track
 * Among others, counts note values distribution and percent of most frequently used value
 * All notes with values between 2n'th dot and n'th dot are considered to be n'th value (say, notes between 8th dot and 4th dot are considered to be 4th value)
 * \param tpq
 * \param lastTick
 */
void MidiTrack::postProcess(const int tpq, const int lastTick) {

    noteCount = notes.size();

    if (finalTick == -1) {
        finalTick = lastTick;
    }

    // Closing all unclosed notes and throwing error
    if (!pendingNoteIndexes.empty()) {
        closeAllNotes(lastTick);
        std::cerr << "Not all notes were closed to the end of track";
    }

    // Counting minNote, maxNote, averageVelocity
    if (!notes.empty()) {
        lowestNote = notes[0]->keyNumber;
        highestNote = notes[0]->keyNumber;
    }

    double velocitySum = 0.0;

    int maxNoteValue = 0;
    for (auto note: notes) {
        int ticks = note->durationInTicks();
        double noteValueRelativeToQuater = 1.0 * ticks / tpq;
        if (noteValueRelativeToQuater < NOTE_VALUE_64TH_DOT_RELATIVE_TO_4TH) {
            note->duration = MidiNote::NoteValue::SIXTY_FOURTH_NOTE;
        } else if (noteValueRelativeToQuater < NOTE_VALUE_32TH_DOT_RELATIVE_TO_4TH) {
            note->duration = MidiNote::NoteValue::THIRTY_SECOND_NOTE;
        } else if (noteValueRelativeToQuater < NOTE_VALUE_16TH_DOT_RELATIVE_TO_4TH) {
            note->duration = MidiNote::NoteValue::SIXTEENTH_NOTE;
        } else if (noteValueRelativeToQuater < NOTE_VALUE_8TH_DOT_RELATIVE_TO_4TH) {
            note->duration = MidiNote::NoteValue::EIGHTH_NOTE;
        } else if (noteValueRelativeToQuater < NOTE_VALUE_4TH_DOT_RELATIVE_TO_4TH) {
            note->duration = MidiNote::NoteValue::QUARTER_NOTE;
        } else if (noteValueRelativeToQuater < NOTE_VALUE_2ND_DOT_RELATIVE_TO_4TH) {
            note->duration = MidiNote::NoteValue::HALF_NOTE;
        } else {
            note->duration = MidiNote::NoteValue::WHOLE_NOTE;
        }

        // Getting distribution of note's length
        MidiNoteDurationDistib::iterator it = durationDistrib.find(static_cast<int>(note->duration));
        if (it == durationDistrib.end()) {
            durationDistrib[static_cast<int>(note->duration)] = 1;
        } else {
            it->second += 1;
            if (it->second > maxNoteValue)
                maxNoteValue = it->second;
        }

        // Updating top and bottom notes
        int noteKey = note->keyNumber;
        if (noteKey > highestNote) {
            highestNote = noteKey;
        } else if (noteKey < lowestNote) {
            lowestNote = noteKey;
        }

        // Updating velocity summ
        velocitySum += note->velocity;
    }
    averageVelocity = velocitySum / noteCount;
    maxNoteValuePercent = 100.0 * maxNoteValue / noteCount;
}

int MidiTrack::durationInTicks() {
    return finalTick - startTick;
}
