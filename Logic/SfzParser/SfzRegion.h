//
// Created by Semyon Tykhonenko on 3/29/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_SFZREGION_H
#define VOCALTRAINER_SFZREGION_H

#include <optional>
#include <string>

struct SfzLoop {
    int start;
    int end;

    SfzLoop(int start, int end) : start(start), end(end) {
    }
};

struct SfzRegion {
    std::string filePath;
    int lowKey = -1;
    int highKey = -1;
    int keyCenter = -1;
    std::optional<SfzLoop> loop;
    bool isLowVelocity = false;
};

#endif //VOCALTRAINER_SFZREGION_H
