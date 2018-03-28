//
// Created by Ivan Kuskov on 3/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef MIDINOTE_H
#define MIDINOTE_H

struct MidiNote
{
    enum Duration {
        D64,
        D32,
        D16,
        D8,
        D4,
        D2,
        D1
    };
    int      keyNumber;
    int      startTick;
    int      finalTick;
    int      velocity;
    Duration duration;

    int durationInTicks() {
        return finalTick - startTick;
    }
};

#endif // MIDINOTE_H
