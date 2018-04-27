//
// Created by Semyon Tikhonenko on 4/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PORTAUDIO_H
#define VOCALTRAINER_PORTAUDIO_H


class PortAudio {
public:
    static void init();
    static void terminate();
    static void checkErrors(int err);
};


#endif //VOCALTRAINER_PORTAUDIO_H
