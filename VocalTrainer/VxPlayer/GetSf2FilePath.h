//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_GETSF2FILEPATH_H
#define VOCALTRAINER_GETSF2FILEPATH_H

#ifdef __APPLE__
#import <string>
std::string GetSf2FilePath();
#else
#include <string>

std::string GetSf2FilePath();
#endif



#endif //VOCALTRAINER_GETSF2FILEPATH_H
