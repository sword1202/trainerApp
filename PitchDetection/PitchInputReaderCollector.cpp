//
// Created by Semyon Tikhonenko on 5/18/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "PitchInputReaderCollector.h"
#include "TimeUtils.h"
#include "AubioPitchDetector.h"
#include "Executors.h"

using namespace CppUtils;

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
        PitchDetector* pitchDetector,
        bool deleteAudioInputReaderOnDestructor) {
    pitchInputReader = new PitchInputReader(audioInputReader, pitchDetector, smoothLevel);
    pitchInputReader->setDestroyAudioInputReaderOnDestructor(deleteAudioInputReaderOnDestructor);

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

bool PitchInputReaderCollector::isRunning() const {
    return pitchInputReader->isRunning();
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

void PitchInputReaderCollector::start() {
    assert(pitchInputReader && "call init before start");
    pitchInputReader->start();
}

void PitchInputReaderCollector::stop() {
    pitchInputReader->stop();
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

int PitchInputReaderCollector::addPitchDetectedListener(const PitchInputReaderCollector::PitchDetectedListener &listener) {
    return pitchDetectedListeners.addListener(listener);
}

void PitchInputReaderCollector::removePitchDetectedListener(int key) {
    pitchDetectedListeners.removeListener(key);
}
