//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "LoadTsf.h"
#import <Foundation/Foundation.h>

tsf * LoadTsf() {
    NSBundle *myBundle = [NSBundle mainBundle];
    NSString *absPath = [myBundle pathForResource:@"a" ofType:@"sf2"];
    return tsf_load_filename(absPath.cString);
}
