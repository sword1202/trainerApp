//
// Created by Semyon Tikhonenko on 7/4/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "VxPitchInputReader.h"
#include "AubioPitchDetector.h"

static constexpr float THRESHOLD = 0.1;
static const int BUFFER_SIZE = 1024;
static const int SMOOTH_LEVEL = 4;

VxPitchInputReader::VxPitchInputReader() {
    AubioPitchDetector* pitchDetector = new AubioPitchDetector();
    pitchDetector->setThreshold(THRESHOLD);
    init(CreateDefaultAudioInputReader(BUFFER_SIZE), SMOOTH_LEVEL, pitchDetector, true);
}
