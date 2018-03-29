//
// Created by Ivan Kuskov on 3/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef MIDINOTE_H
#define MIDINOTE_H

struct MidiNote
{
    enum class NoteValue : int {
		SIXTY_FOURTH_NOTE = 0x0,
        THIRTY_SECOND_NOTE = 0x1,
        SIXTEENTH_NOTE = 0x2,
        EIGHTH_NOTE  = 0x3,
        QUARTER_NOTE  = 0x4,
        HALF_NOTE  = 0x5,
        WHOLE_NOTE  = 0x6
    };
    int      keyNumber;
    int      startTick;
    int      finalTick;
    int      velocity;
    NoteValue duration;

    int durationInTicks() {
        return finalTick - startTick;
    }
};

#endif // MIDINOTE_H
