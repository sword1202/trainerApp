//
// Created by Semyon Tykhonenko on 3/29/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_SFZFILE_H
#define VOCALTRAINER_SFZFILE_H

#include "SfzRegion.h"
#include "Pitch.h"
#include <vector>

class SfzFile {
    std::vector<SfzRegion> regions;
public:
    typedef std::function<AudioData(const std::string&)> SampleReader;

    SfzFile(const std::string& data, const SampleReader& sampleReader);
    const SfzRegion& findRegion(const Pitch& pitch) const;
};


#endif //VOCALTRAINER_SFZFILE_H
