//
// Created by Semyon Tykhonenko on 3/29/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_SFZFILE_H
#define VOCALTRAINER_SFZFILE_H

#include "SfzRegion.h"
#include <vector>

class SfzFile {
    std::vector<SfzRegion> regions;
public:
    SfzFile(const std::string& data);
};


#endif //VOCALTRAINER_SFZFILE_H
