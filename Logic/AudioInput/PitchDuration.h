//
// Created by Semyon Tykhonenko on 1/19/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_PITCHDURATION_H
#define VOCALTRAINER_PITCHDURATION_H


class PitchDuration {
public:
    static constexpr int WHOLE = 1;
    static constexpr int WHOLE_WITH_DOT = -1;
    static constexpr int HALF = 2;
    static constexpr int HALF_WITH_DOT = -2;
    static constexpr int QUARTER = 4;
    static constexpr int QUARTER_WITH_DOT = -4;
    static constexpr int EIGHTH = 8;
    static constexpr int EIGHTH_WITH_DOT = -8;
    static constexpr int SIXTEENTH = 16;
    static constexpr int SIXTEENTH_WITH_DOT = -16;
    static constexpr int THIRTY = 32;
    static constexpr int THIRTY_WITH_DOT = -32;

    static double getBeatsCount(int duration);
    static int getRequiredTicksPerBeat(int duration);
    static bool isValid(int duration);
};


#endif //VOCALTRAINER_PITCHDURATION_H
