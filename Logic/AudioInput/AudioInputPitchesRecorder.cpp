//
// Created by Semyon Tikhonenko on 5/18/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "AudioInputPitchesRecorder.h"
#include "TimeUtils.h"
#include "Executors.h"
#include "Algorithms.h"
#include <iostream>

using namespace CppUtils;
using namespace std;

#define LOCK std::lock_guard<std::mutex> _(mutex)

void AudioInputPitchesRecorder::init(AudioInputReader *audioInputReader, int smoothLevel,
        PitchDetector* pitchDetector) {
    pitchInputReader = new PitchInputReader(audioInputReader, pitchDetector, smoothLevel);

    pitchInputReader->setExecuteCallBackOnInvalidPitches(true);
    pitchInputReader->setCallback([=](const Pitch& pitch) {
        float frequency = pitch.getFrequency();
        double seek = pitches.appendPitch(frequency);
        pitchDetected(frequency, seek);
    });
}

void AudioInputPitchesRecorder::operator()(const int16_t* data, int size) {
    assert(pitchInputReader && "call init before");
    pitchInputReader->operator()(data, size);
}

AudioInputPitchesRecorder::~AudioInputPitchesRecorder() {
    delete pitchInputReader;
}

void AudioInputPitchesRecorder::pitchDetected(float frequency, double time) {
    executeOnMainThread([=] {
        pitchDetectedListeners.executeAll(Pitch(frequency), time);
    });
}

void AudioInputPitchesRecorder::setSeek(double seek) {
    pitches.setSeek(seek);
}

const PitchesCollection* AudioInputPitchesRecorder::getPitches() const {
    return &pitches;
}

void AudioInputPitchesRecorder::clearRecordedPitches() {
    pitches.clearPitches();
}
