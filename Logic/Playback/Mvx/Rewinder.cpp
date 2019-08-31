//
// Created by Semyon Tykhonenko on 8/29/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#include "Rewinder.h"
#include "TimeUtils.h"
#include "MathUtils.h"

using namespace CppUtils;
using namespace Math;

void Rewinder::startRewind(bool backward) {
    assert(!timer.isRunning() && "forward, backward rewind is already running");
    this->backward = backward;

    timer.onStop = [=] {
        double timeDiff = (TimeUtils::NowInMicroseconds() - timer.getTimerStartedTime()) / 1000000.0;
        double postRewindDuration = minimumRewindDuration - timeDiff;
        if (postRewindDuration > 0) {
            rewindWithDuration(postRewindDuration);
        }
    };

    timer.start(RoundToInt(rewindInterval * 1000), [=] {
        double timeDiff = (TimeUtils::NowInMicroseconds() - timer.getLastIterationTimeInMicroseconds()) / 1000000.0;
        rewindWithDuration(timeDiff);
    }, RoundToInt(initialDelay * 1000));
}

void Rewinder::stopRewind() {
    timer.stop();
}

bool Rewinder::isRewindRunning(bool *backward) const {
    if (!timer.isRunning()) {
        return false;
    }

    if (backward) {
        *backward = this->backward;
    }

    return true;
}

double Rewinder::getMinimumRewindDuration() const {
    return minimumRewindDuration;
}

void Rewinder::setMinimumRewindDuration(double minimumRewindDuration) {
    this->minimumRewindDuration = minimumRewindDuration;
}

double Rewinder::getInitialDelay() const {
    return initialDelay;
}

void Rewinder::setInitialDelay(double initialDelay) {
    this->initialDelay = initialDelay;
}

double Rewinder::getRewindInterval() const {
    return rewindInterval;
}

void Rewinder::setRewindInterval(double rewindInterval) {
    this->rewindInterval = rewindInterval;
}

double Rewinder::getRewindSpeedFactor() const {
    return rewindSpeedFactor;
}

void Rewinder::setRewindSpeedFactor(double rewindSpeedFactor) {
    this->rewindSpeedFactor = rewindSpeedFactor;
}

void Rewinder::rewindWithDuration(double duration) {
    double seek = rewindable->getSeek();
    seek += backward ? -duration * rewindSpeedFactor : duration * rewindSpeedFactor;
    seek -= duration;

    if (seek > rewindable->getDuration()) {
        seek = rewindable->getDuration();
    }

    if (seek < 0) {
        seek = 0;
    }

    rewindable->setSeek(seek);
}

Rewinder::Rewinder(Rewindable *rewindable) : rewindable(rewindable) {
}
