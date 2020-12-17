//
// Created by Semyon Tykhonenko on 1/19/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#include "PitchDuration.h"
#include <cstdlib>
#include <assert.h>
#include "Algorithms.h"

double PitchDuration::getBeatsCount(int duration) {
    assert(duration != 0);
    if (duration > 0) {
        return 1.0 / duration * 4;
    } else {
        int absDuration = abs(duration);
        return (1.0 / absDuration + 1.0 / absDuration / 2.0) * 4.0;
    }
}

int PitchDuration::getRequiredTicksPerBeat(int duration) {
    switch (duration) {
        case WHOLE:
        case WHOLE_WITH_DOT:
        case HALF:
        case HALF_WITH_DOT:
        case QUARTER:
            return 1;
        case QUARTER_WITH_DOT:
        case EIGHTH:
            return 2;
        case EIGHTH_WITH_DOT:
        case SIXTEENTH:
            return 4;
        case SIXTEENTH_WITH_DOT:
        case THIRTY_SECOND:
            return 8;
        case THIRTY_SECOND_WITH_DOT:
            return 16;
        default:
            assert(false && "Duration not supported");
    }

    return 0;
}

bool PitchDuration::isValid(int duration) {
    static std::vector<int> values { WHOLE, WHOLE_WITH_DOT, HALF, HALF_WITH_DOT, QUARTER, QUARTER_WITH_DOT, EIGHTH,
            EIGHTH_WITH_DOT, SIXTEENTH, SIXTEENTH_WITH_DOT, THIRTY_SECOND, THIRTY_SECOND_WITH_DOT };
    return CppUtils::ContainsValue(values, duration);
}
