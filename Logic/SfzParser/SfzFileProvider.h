//
// Created by Semyon Tikhonenko on 11/29/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_SFZFILEPROVIDER_H
#define VOCALTRAINER_SFZFILEPROVIDER_H

#include "AudioData.h"
#include "SfzFile.h"

class SfzFileProvider {
public:
    static SfzFile* get();
};


#endif //VOCALTRAINER_SFZFILEPROVIDER_H
