//
// Created by Semyon Tikhonenko on 5/18/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "AudioInputPitchesRecorder.h"
#include "TimeUtils.h"
#include "AubioPitchDetector.h"
#include "Executors.h"
#include <iostream>

using namespace CppUtils;
using namespace std;

#define LOCK std::lock_guard<std::mutex> _(mutex)

float AudioInputPitchesRecorder::getFrequencyAt(int index) const {
    LOCK;
    return frequencies[index];
}

double AudioInputPitchesRecorder::getTimeAt(int index) const {
    LOCK;
    return times[index];
}

int AudioInputPitchesRecorder::getPitchesCount() const {
    LOCK;
    return frequencies.size();
}

void AudioInputPitchesRecorder::init(AudioInputReader *audioInputReader, int smoothLevel,
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

void AudioInputPitchesRecorder::operator()(const int16_t* data, int size) {
    assert(pitchInputReader && "call init before");
    pitchInputReader->operator()(data, size);
}

void AudioInputPitchesRecorder::setThreshold(float threshold) {
    pitchInputReader->getPitchDetector()->setThreshold(threshold);
}

float AudioInputPitchesRecorder::getThreshold() const {
    return pitchInputReader->getPitchDetector()->getThreshold();
}

AudioInputPitchesRecorder::~AudioInputPitchesRecorder() {
    delete pitchInputReader;
}

void AudioInputPitchesRecorder::pitchDetected(float frequency, double time) {
    Executors::ExecuteOnMainThread([=] {
        pitchDetectedListeners.executeAll(Pitch(frequency), time);
    });
}

double AudioInputPitchesRecorder::getSavedPitchesTimeLimit() const {
    return savedPitchesTimeLimit;
}

void AudioInputPitchesRecorder::setSavedPitchesTimeLimit(double savedPitchesTimeLimit) {
    this->savedPitchesTimeLimit = savedPitchesTimeLimit;
}

float AudioInputPitchesRecorder::getLastDetectedFrequency() const {
    LOCK;
    if (frequencies.empty()) {
        return -1;
    }

    return frequencies.front();
}

Pitch AudioInputPitchesRecorder::getLastDetectedPitch() const {
    return Pitch(getLastDetectedFrequency());
}

double AudioInputPitchesRecorder::getLastDetectedTime() const {
    LOCK;
    if (times.empty()) {
        return -1;
    }

    return times.front();
}

void AudioInputPitchesRecorder::clearCollectedPitches() {
    LOCK;
    frequencies.clear();
    times.clear();
}

int AudioInputPitchesRecorder::getPitchesCountAfterTime(double time) const {
    auto iter = std::upper_bound(times.begin(), times.end(), time, std::greater<>());
    if (iter == times.end()) {
        return getPitchesCount();
    }

    return static_cast<int>(iter - times.begin());
}