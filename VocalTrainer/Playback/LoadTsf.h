//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_GETSF2FILEPATH_H
#define VOCALTRAINER_GETSF2FILEPATH_H

#include <string>

#ifdef _WIN32
 #include "tsf.h"
#else
 #import "tsf.h"
#endif


tsf* LoadTsf();

#endif //VOCALTRAINER_GETSF2FILEPATH_H
