//
// Created by Semyon Tikhonenko on 1/11/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

@interface NSAlert (AlertUtils)
+ (NSModalResponse)error:(NSString *)message;
@end