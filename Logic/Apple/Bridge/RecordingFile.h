//
// Created by Semyon Tikhonenko on 8/24/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#import <Foundation/Foundation.h>
#ifdef __cplusplus
#include "VocalTrainerFile.h"
#endif

@interface RecordingFile : NSObject
@property (readonly) NSTimeInterval creationTime;
#ifdef __cplusplus
@property (readonly) VocalTrainerFile* file;
- (instancetype)initWithFile:(VocalTrainerFile *)file;
#endif
@end