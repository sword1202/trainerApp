//
// Created by Semyon Tykhonenko on 3/29/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_SFZSECTIONSPARSER_H
#define VOCALTRAINER_SFZSECTIONSPARSER_H

#include <vector>
#include "SfzSection.h"

class SfzSectionsParser {
public:
    std::vector<SfzSection> parse(const std::string& data);
};

#endif //VOCALTRAINER_SFZSECTIONSPARSER_H
