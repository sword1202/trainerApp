//
// Created by Semyon Tikhonenko on 7/3/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "Manager.h"

Manager* Manager::_instance = nullptr;

Manager *Manager::instance() {
    return _instance;
}

void Manager::init() {
    _instance = new Manager();
}

Manager::Manager() {
    pitchInputReader = new PitchInputReaderCollector();
    pitchInputReader->init(CreateDefaultAudioInputReader(2048), 1, true);
}

PitchInputReaderCollector *Manager::getPitchInputReader() const {
    return pitchInputReader;
}
