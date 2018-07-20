//
// Created by Semyon Tikhonenko on 7/7/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

IB_DESIGNABLE
@interface SVGImageView : NSView
@property (nonatomic) IBInspectable NSString *fileName;

- (instancetype)initWithFileName:(NSString *)fileName andFrame:(CGRect)frame;


@end