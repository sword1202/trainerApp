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

    static std::vector<const PaDeviceInfo*> getInputDevices();
};


#endif //VOCALTRAINER_PORTAUDIO_H
