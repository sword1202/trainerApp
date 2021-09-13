//
// Created by Semyon Tikhonenko on 5/10/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Logic/Logic-Swift.h>
#include "SingingCompletionFlow.h"

@interface SingingCompletionFlowObjCWrapper : NSObject<SingingCompletionFlowBridge>
- (instancetype)initWithCpp:(SingingCompletionFlow *)cpp;
@end