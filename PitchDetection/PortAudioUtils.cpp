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

std::vector<const PaDeviceInfo*> PortAudio::getInputDevices() {
    std::vector<const PaDeviceInfo*> result;
    int deviceCount = Pa_GetDeviceCount();
    for (int i = 0; i < deviceCount; ++i) {
        const auto* info = Pa_GetDeviceInfo(i);
        if (info->maxInputChannels > 0) {
            result.push_back(info);
        }
    }

    return result;
}
