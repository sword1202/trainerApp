//
// Created by Semyon Tikhonenko on 9/17/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_INTERCONTROLLERCOMMUNICATIONEVENTS_H
#define VOCALTRAINER_INTERCONTROLLERCOMMUNICATIONEVENTS_H

#include "ListenersSet.h"

class InterControllerCommunicationEvents {
public:
    inline static InterControllerCommunicationEvents& instance() {
        static InterControllerCommunicationEvents _;
        return _;
    }

    CppUtils::ListenersSet<> onNewRecordingAdded;
};


#endif //VOCALTRAINER_INTERCONTROLLERCOMMUNICATIONEVENTS_H
