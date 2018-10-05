//
// Created by Semyon Tikhonenko on 4/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PORTAUDIO_H
#define VOCALTRAINER_PORTAUDIO_H

#include <portaudio/portaudio.h>
#include <vector>

class PortAudio {
public:
    static void init();
    static void terminate();
    static void checkErrors(PaError err);

    static bool hasInputDevices();
    static std::vector<const PaDeviceInfo*> getInputDevices();
    static const PaDeviceInfo* findInputDeviceByName(const char* name, int* outIndex = nullptr);
};


#endif //VOCALTRAINER_PORTAUDIO_H
