//
// Created by Semyon Tikhonenko on 7/30/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#include "TimeSignature.h"
#include "PitchDuration.h"
#include <assert.h>

int TimeSignature::getBeatDuration() const {
    return beatDuration;
}

int TimeSignature::getNumberOfBeatsInBar() const {
    return numberOfBeatsInBar;
}

TimeSignature::TimeSignature(int numberOfBeatsInBar, int beatDuration)
        : beatDuration(beatDuration), numberOfBeatsInBar(numberOfBeatsInBar) {
    assert(numberOfBeatsInBar >= 1);
    assert(PitchDuration::isValid(beatDuration));
}

TimeSignature::TimeSignature() : TimeSignature(4, 4) {
}
