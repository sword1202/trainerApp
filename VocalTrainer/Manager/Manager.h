//
// Created by Semyon Tikhonenko on 7/3/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_MANAGER_H
#define VOCALTRAINER_MANAGER_H


#include "PitchInputReaderCollector.h"

class Manager {
    static Manager* _instance;
    PitchInputReaderCollector* pitchInputReader;
public:
    Manager();
    PitchInputReaderCollector *getPitchInputReader() const;

    static void init();
    static Manager* instance();
};


#endif //VOCALTRAINER_MANAGER_H
