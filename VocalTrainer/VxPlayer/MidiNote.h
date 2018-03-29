//
// Created by Ivan Kuskov on 3/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef MIDINOTE_H
#define MIDINOTE_H

struct MidiNote
{
    enum class Value : int {
        D64 = 0x0,
        D32 = 0x1,
        D16 = 0x2,
        D8  = 0x3,
        D4  = 0x4,
        D2  = 0x5,
        D1  = 0x6
    };
    int      keyNumber;
    int      startTick;
    int      finalTick;
    int      velocity;
    Value duration;

    int durationInTicks() {
        return finalTick - startTick;
    }
};

#endif // MIDINOTE_H
