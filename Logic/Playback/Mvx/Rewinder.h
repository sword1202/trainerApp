//
// Created by Semyon Tykhonenko on 8/29/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_REWINDER_H
#define VOCALTRAINER_REWINDER_H

#include "Rewindable.h"
#include "Timer.h"

class Rewinder {
    bool backward = false;
    Rewindable* rewindable;
    CppUtils::Timer timer;
    double minimumRewindDuration = 2.0;
    double initialDelay = 0.5;
    double rewindInterval = 0.5;
    double rewindSpeedFactor = 5;

    void rewindWithDuration(double duration);
public:
    Rewinder(Rewindable *rewindable);

    double getMinimumRewindDuration() const;
    void setMinimumRewindDuration(double minimumRewindDuration);

    double getInitialDelay() const;
    void setInitialDelay(double initialDelay);

    double getRewindInterval() const;
    void setRewindInterval(double rewindInterval);

    double getRewindSpeedFactor() const;
    void setRewindSpeedFactor(double rewindSpeedFactor);

    void startRewind(bool backward);
    void stopRewind();
    bool isRewindRunning(bool* backward = nullptr) const;
};


#endif //VOCALTRAINER_REWINDER_H
