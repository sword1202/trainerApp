//
// Created by Semyon Tykhonenko on 3/29/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_SFZSECTION_H
#define VOCALTRAINER_SFZSECTION_H

#include <string>
#include <map>

struct SfzSection {
    using Attrs = std::map<std::string, std::string>;
    std::string name;
    Attrs attrs;
};


#endif //VOCALTRAINER_SFZSECTION_H
