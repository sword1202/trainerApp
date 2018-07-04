//
// Created by Semyon Tikhonenko on 7/3/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "Manager.h"
#include "AubioPitchDetector.h"

Manager* Manager::_instance = nullptr;

Manager *Manager::instance() {
    return _instance;
}

void Manager::init() {
    _instance = new Manager();
}

Manager::Manager() {
    pitchInputReader = new PitchInputReaderCollector();
    AubioPitchDetector* pitchDetector = new AubioPitchDetector();
    pitchDetector->setThreshold(0.1);
    pitchInputReader->init(CreateDefaultAudioInputReader(1024), 4, pitchDetector, true);
}

PitchInputReaderCollector *Manager::getPitchInputReader() const {
    return pitchInputReader;
}
