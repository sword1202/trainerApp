//
// Created by Semyon Tikhonenko on 8/24/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#import "PlaybackSource.h"
#include "Streams.h"
#include "FileUtils.h"

using namespace CppUtils;

@implementation PlaybackSource {
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

- (instancetype)initWithFilePath:(NSString *)filePath {
    self = [super init];
    if (self) {
        const char *path = filePath.UTF8String;
        std::fstream is = Streams::OpenFile(path, std::ios::in | std::ios::binary);
        _file = VocalTrainerFile::read(is);
        _creationTime = FileUtils::GetLastWriteTimeInMicroseconds(path) / 1000000.0;
    }

    return self;
}


- (NSTimeInterval)creationTime {
    return _creationTime;
}

- (VocalTrainerFile*)file {
    auto* result = _file;
    _file = nullptr;
    return result;
}

- (void)dealloc {
    delete _file;
}

@end