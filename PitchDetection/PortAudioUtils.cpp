//
// Created by Semyon Tikhonenko on 4/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "PortAudioUtils.h"
#include <stdexcept>
#include <string>

void PortAudio::init() {
    PaError err = Pa_Initialize();
    checkErrors(err);
}

void PortAudio::terminate() {
    PaError err = Pa_Terminate();
    checkErrors(err);
}

void PortAudio::checkErrors(PaError err) {
    if (err != paNoError) {
        auto error = std::string("PortAudio error: ") + Pa_GetErrorText(err);
        throw std::runtime_error(error);
    }
}
