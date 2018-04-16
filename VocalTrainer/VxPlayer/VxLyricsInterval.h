//
// Created by Semyon Tikhonenko on 4/16/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXLYRICSINTERVAL_H
#define VOCALTRAINER_VXLYRICSINTERVAL_H


#import <assert.h>

struct VxLyricsInterval {
    int letterStartIndex;
    int letterEndIndex;
    int startTickNumber;
    int ticksCount;
};


#endif //VOCALTRAINER_VXLYRICSINTERVAL_H
