//
// Created by Semyon Tikhonenko on 12/6/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOTOOLBOXUTILS_H
#define VOCALTRAINER_AUDIOTOOLBOXUTILS_H

#include <AudioToolbox/AudioToolbox.h>
#include "AudioStreamDescription.h"

class AudioToolboxUtils {
public:
    static void throwExceptionIfError(OSStatus status);
    static void createFormat(const AudioStreamDescription& description, AudioStreamBasicDescription* out);
};


#endif //VOCALTRAINER_AUDIOTOOLBOXUTILS_H
