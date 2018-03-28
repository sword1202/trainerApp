//
// Created by Ivan Kuskov on 3/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef MIDITRACK_H
#define MIDITRACK_H

#include "midinote.h"

struct MidiTrack
{
    std::string           instrumentName;
    std::string           trackName;
    bool                  isPolyphonical;
    int                   channelId;
    int                   trackId;
    int                   noteCount;
    int                   lowestNote;
    int                   highestNote;
    double                averageVelocity;
    double                maxNoteLengthPercent;
    double                durationInTime;

    int                   startTick = -1;
    int                   finalTick = -1;

    MidiTrack() :
        startTick(-1),
        finalTick(-1)
    {
    }

private:
    using MidiNotePointersVector = std::vector<std::shared_ptr<MidiNote> >;
    using MidiNotePointersList   = std::list  <std::shared_ptr<MidiNote> >;
    using MidiNoteDurationDistib = std::map   <MidiNote::Duration, int>;
    MidiNotePointersVector m_notes;
    MidiNotePointersList   m_pendingNotes;
    MidiNoteDurationDistib m_durationDistrib;

public:
    MidiNotePointersVector &getNotes() {
        return m_notes;
    }

    void clear() {
        startTick = -1;
        finalTick = -1;
        instrumentName = "";
        trackName = "";
        isPolyphonical = false;
        channelId = -1;
        trackId = -1;
        noteCount = -1;
        lowestNote = -1;
        highestNote = -1;
        averageVelocity = 0.0;
        maxNoteLengthPercent = 0.0;
        durationInTime = 0.0;
        m_notes.clear();
        m_pendingNotes.clear();
        m_durationDistrib.clear();
    }

    void openNote(const int keyNumber, const int tick, const int velocity) {
        std::shared_ptr<MidiNote> note = std::make_shared<MidiNote>();
        note->startTick = tick;
        note->keyNumber = keyNumber;
        note->velocity = velocity;
        m_notes.push_back(note);

        // If the same note is already in pending, closing it and opening new
        MidiNotePointersList::iterator it;
        for (it = m_pendingNotes.begin(); it != m_pendingNotes.end();) {
            if (it->get()->keyNumber == keyNumber) {
                closeNote(keyNumber, tick);
            } else {
                ++it;
            }
        }
        if (m_pendingNotes.size() > 0) {
            isPolyphonical = true;
        }
        m_pendingNotes.push_back(note);

        if (startTick == -1) {
            startTick = tick;
        }

    }

    void closeNote(const int keyNumber, const int tick) {
        MidiNotePointersList::iterator it;
        for (it = m_pendingNotes.begin(); it != m_pendingNotes.end();) {
            MidiNote *note = it->get();
            if (note->keyNumber == keyNumber) {
                note->finalTick = tick;
                it = m_pendingNotes.erase(it);
            } else {
                ++it;
            }
        }
    }
    void closeAllNotes(const int tick) {
        for (auto note : m_pendingNotes) {
            closeNote(note->keyNumber, tick);
        }
    }

    void postProcess(const int tpq, const int lastTick) {

        noteCount = m_notes.size();

        if (finalTick == -1) {
            finalTick = lastTick;
        }

        // Closing all unclosed notes and throwing error
        if (!m_pendingNotes.empty()) {
            closeAllNotes(lastTick);
            std::cerr << "Not all notes were closed to the end of track";
        }

        // Counting minNote, maxNote, averageVelocity
        if (!m_notes.empty()) {
            lowestNote = m_notes[0]->keyNumber;
            highestNote = m_notes[0]->keyNumber;
        }

        double velocitySum = 0.0;

        int maxNoteLength = 0;
        for (auto note: m_notes) {
            int ticks = note->durationInTicks();
            double fraq = 1.0 * ticks / tpq;
            if (fraq < 3. / 32.) {
                note->duration = MidiNote::Duration::D64;
            } else if (fraq < 3. / 16.) {
                note->duration = MidiNote::Duration::D32;
            } else if (fraq < 3. / 8.) {
                note->duration = MidiNote::Duration::D16;
            } else if (fraq < 3. / 4.) {
                note->duration = MidiNote::Duration::D8;
            } else if (fraq < 3. / 2.) {
                note->duration = MidiNote::Duration::D4;
            } else if (fraq < 3. / 1.) {
                note->duration = MidiNote::Duration::D2;
            } else {
                note->duration = MidiNote::Duration::D1;
            }

            // Getting distribution of note's length
            MidiNoteDurationDistib::iterator it = m_durationDistrib.find(note->duration);
            if (it == m_durationDistrib.end()) {
                m_durationDistrib[note->duration] = 1;
            } else {
                it->second += 1;
                if (it->second > maxNoteLength)
                    maxNoteLength = it->second;
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
        maxNoteLengthPercent = 100.0 * maxNoteLength / noteCount;
    }

    int durationInTicks() {
        return finalTick - startTick;
    }
};

#endif // MIDITRACK_H
