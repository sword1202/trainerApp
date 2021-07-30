//
// Created by Semyon Tikhonenko on 7/28/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

@interface NativeColorUtils : NSObject
+ (UInt32)rotateColorPalette:(CGFloat)angle color:(UInt32)color;
@end