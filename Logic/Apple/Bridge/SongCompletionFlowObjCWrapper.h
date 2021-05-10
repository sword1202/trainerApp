//
// Created by Semyon Tikhonenko on 5/10/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Logic/Logic-Swift.h>
#include "SongCompletionFlow.h"

@interface SongCompletionFlowObjCWrapper : NSObject<SongCompletionFlowBridge>
- (instancetype)initWithCpp:(SongCompletionFlow *)cpp;
+ (instancetype)wrapperWithCpp:(SongCompletionFlow *)cpp;

@end