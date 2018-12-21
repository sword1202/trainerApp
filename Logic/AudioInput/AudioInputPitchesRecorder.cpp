//
// Created by Semyon Tikhonenko on 5/18/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "AudioInputPitchesRecorder.h"
#include "TimeUtils.h"
#include "AubioPitchDetector.h"
#include "Executors.h"
#include "Algorithms.h"
#include <iostream>

using namespace CppUtils;
using namespace std;

#define LOCK std::lock_guard<std::mutex> _(mutex)

float AudioInputPitchesRecorder::getFrequencyAt(int index) const {
    LOCK;
    return frequencies[frequencies.size() - index - 1];
}

double AudioInputPitchesRecorder::getTimeAt(int index) const {
    LOCK;
    return times[times.size() - index - 1];
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
        double seek = this->seek;

        {
            LOCK;
            frequencies.push_back(frequency);
            times.push_back(seek);
        }

        pitchDetected(frequency, seek);
    });
    seek = 0;
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

float AudioInputPitchesRecorder::getLastDetectedFrequency() const {
    LOCK;
    if (frequencies.empty()) {
        return -1;
    }

    return frequencies.back();
}

Pitch AudioInputPitchesRecorder::getLastDetectedPitch() const {
    return Pitch(getLastDetectedFrequency());
}

double AudioInputPitchesRecorder::getLastDetectedTime() const {
    LOCK;
    if (times.empty()) {
        return -1;
    }

    return times.back();
}

int AudioInputPitchesRecorder::getPitchesCountAfterTime(double time) const {
    auto iter = std::upper_bound(times.begin(), times.end(), time);
    return static_cast<int>(times.end() - iter);
}

double AudioInputPitchesRecorder::getSeek() const {
    return seek;
}

void AudioInputPitchesRecorder::setSeek(double seek) {
    this->seek = seek;

    LOCK;
    if (!times.empty() && seek < times.back()) {
        // remove all pitches after seek
        auto iter = std::lower_bound(times.begin(), times.end(), seek);
        times.erase(iter, times.end());
        int index = static_cast<int>(times.end() - iter);
        CppUtils::EraseEndingOfCollection(frequencies, index);
    }
}

const std::vector<double> &AudioInputPitchesRecorder::getTimesInAscAddedTimeOrder() {
    return times;
}

const std::vector<float> &AudioInputPitchesRecorder::getFrequenciesInAscAddedTimeOrder() {
    return frequencies;
}
