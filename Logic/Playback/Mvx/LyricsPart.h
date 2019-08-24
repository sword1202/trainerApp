//
// Created by Semyon Tikhonenko on 5/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXLYRICSLINE_H
#define VOCALTRAINER_VXLYRICSLINE_H

#include "LyricsInterval.h"
#include <string>
#include <vector>

struct LyricsPart {
    std::string name;
    std::vector<LyricsInterval> intervals;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & intervals;
    }

    bool operator==(const LyricsPart &rhs) const {
        return name == rhs.name &&
               intervals == rhs.intervals;
    }

    bool operator!=(const LyricsPart &rhs) const {
        return !(rhs == *this);
    }
};


#endif //VOCALTRAINER_VXLYRICSLINE_H
