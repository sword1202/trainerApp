//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "GetSf2FilePath.h"
#import <Foundation/Foundation.h>

std::string GetSf2FilePath() {
    NSBundle *myBundle = [NSBundle mainBundle];
    NSString *absPath= [myBundle pathForResource:@"a" ofType:@"sf2"];
    return absPath.cString;
}
