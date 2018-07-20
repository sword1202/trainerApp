//
// Created by Semyon Tikhonenko on 7/7/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

@interface PlayHeadView : NSView

@property (assign, nonatomic) CGFloat position;

- (instancetype)initWithHeight:(CGFloat)height;
@end