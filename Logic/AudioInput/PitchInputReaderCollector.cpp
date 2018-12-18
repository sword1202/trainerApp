//
// Created by Semyon Tikhonenko on 5/18/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "PitchInputReaderCollector.h"
#include "TimeUtils.h"
#include "AubioPitchDetector.h"
#include "Executors.h"
#include <iostream>

using namespace CppUtils;
using namespace std;

#define LOCK std::lock_guard<std::mutex> _(mutex)

float PitchInputReaderCollector::getFrequencyAt(int index) const {
    LOCK;
    return frequencies[index];
}

double PitchInputReaderCollector::getTimeAt(int index) const {
    LOCK;
    return times[index];
}

int PitchInputReaderCollector::getPitchesCount() const {
    LOCK;
    return frequencies.size();
}

void PitchInputReaderCollector::init(AudioInputReader *audioInputReader, int smoothLevel,
        PitchDetector* pitchDetector) {
    pitchInputReader = new PitchInputReader(audioInputReader, pitchDetector, smoothLevel);

    pitchInputReader->setExecuteCallBackOnInvalidPitches(true);
    pitchInputReader->setCallback([=](const Pitch& pitch) {
        float frequency = pitch.getFrequency();
        double time = TimeUtils::NowInSeconds();

        {
            LOCK;
            frequencies.push_front(frequency);
            times.push_front(time);
            if (times.front() - times.back() > savedPitchesTimeLimit) {
                times.pop_back();
                frequencies.pop_back();
            }
        }

        pitchDetected(frequency, time);
    });
}

void PitchInputReaderCollector::operator()(const int16_t* data, int size) {
    assert(pitchInputReader && "call init before");
    pitchInputReader->operator()(data, size);
}

void PitchInputReaderCollector::setThreshold(float threshold) {
    pitchInputReader->getPitchDetector()->setThreshold(threshold);
}

float PitchInputReaderCollector::getThreshold() const {
    return pitchInputReader->getPitchDetector()->getThreshold();
}

PitchInputReaderCollector::~PitchInputReaderCollector() {
    delete pitchInputReader;
}

void PitchInputReaderCollector::pitchDetected(float frequency, double time) {
    Executors::ExecuteOnMainThread([=] {
        pitchDetectedListeners.executeAll(Pitch(frequency), time);
    });
}

double PitchInputReaderCollector::getSavedPitchesTimeLimit() const {
    return savedPitchesTimeLimit;
}

void PitchInputReaderCollector::setSavedPitchesTimeLimit(double savedPitchesTimeLimit) {
    this->savedPitchesTimeLimit = savedPitchesTimeLimit;
}

float PitchInputReaderCollector::getLastDetectedFrequency() const {
    LOCK;
    if (frequencies.empty()) {
        return -1;
    }

    return frequencies.front();
}

Pitch PitchInputReaderCollector::getLastDetectedPitch() const {
    return Pitch(getLastDetectedFrequency());
}

double PitchInputReaderCollector::getLastDetectedTime() const {
    LOCK;
    if (times.empty()) {
        return -1;
    }

    return times.front();
}

void PitchInputReaderCollector::clearCollectedPitches() {
    LOCK;
    frequencies.clear();
    times.clear();
}

int PitchInputReaderCollector::getPitchesCountAfterTime(double time) const {
    auto iter = std::upper_bound(times.begin(), times.end(), time, std::greater<>());
    if (iter == times.end()) {
        return getPitchesCount();
    }

    return static_cast<int>(iter - times.begin());
}