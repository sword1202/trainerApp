//
// Created by Semyon Tykhonenko on 2019-07-28.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface InitializationManager : NSObject
+ (void)initializeApplicationIfNeed;
+ (BOOL)isInitialized;
@end