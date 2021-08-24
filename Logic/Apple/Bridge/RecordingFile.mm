//
// Created by Semyon Tikhonenko on 8/24/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#import "RecordingFile.h"
#include "TimeUtils.h"

using namespace CppUtils;

@implementation RecordingFile {
    NSTimeInterval _creationTime;
    VocalTrainerFile* _file;
}
- (instancetype)initWithFile:(VocalTrainerFile *)file {
    self = [super init];
    if (self) {
        _file = file;
        _creationTime = [[NSDate date] timeIntervalSince1970];
    }

    return self;
}

- (NSTimeInterval)creationTime {
    return _creationTime;
}

- (VocalTrainerFile*)file {
    return _file;
}

@end