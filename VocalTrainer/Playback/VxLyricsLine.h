//
// Created by Semyon Tikhonenko on 5/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXLYRICSLINE_H
#define VOCALTRAINER_VXLYRICSLINE_H

#include "VxLyricsInterval.h"
#include <string>
#include <vector>

struct VxLyricsLine {
    std::string name;
    std::vector<VxLyricsInterval> intervals;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & intervals;
    }
};


#endif //VOCALTRAINER_VXLYRICSLINE_H
