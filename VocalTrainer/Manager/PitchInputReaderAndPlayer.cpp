//
// Created by Semyon Tikhonenko on 7/4/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "PitchInputReaderAndPlayer.h"
#include "AubioPitchDetector.h"
#include "Executors.h"

static constexpr float THRESHOLD = 0.1;
static const int BUFFER_SIZE = 1024;
static const int SMOOTH_LEVEL = 4;

using namespace CppUtils;

PitchInputReaderAndPlayer::PitchInputReaderAndPlayer() {
    AubioPitchDetector* pitchDetector = new AubioPitchDetector();
    pitchDetector->setThreshold(THRESHOLD);

    audioInputReader = CreateDefaultAudioInputReaderWithOutput(BUFFER_SIZE);

    init(audioInputReader, SMOOTH_LEVEL, pitchDetector, true);
}

void PitchInputReaderAndPlayer::pitchDetected(float frequency, double time) {
    CppUtils::Executors::ExecuteOnMainThread([=] {
        PitchInputReaderCollector::pitchDetected(frequency, time);
    });
}

PitchInputReaderAndPlayer::~PitchInputReaderAndPlayer() {
}

void PitchInputReaderAndPlayer::setInputVolume(float value) {
    audioInputReader->setInputVolume(value);
}

float PitchInputReaderAndPlayer::getInputVolume() const {
    return audioInputReader->getInputVolume();
}

void PitchInputReaderAndPlayer::setOutputVolume(float value) {
    audioInputReader->setOutputVolume(value);
}

float PitchInputReaderAndPlayer::getOutputVolume() const {
    return audioInputReader->getOutputVolume();
}
