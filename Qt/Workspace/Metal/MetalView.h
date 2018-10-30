//
// Created by Semyon Tikhonenko on 10/25/18.
// Copyright (c) 2018 ___FULLUSERNAME___. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MetalKit/MetalKit.h>
#include "MetalViewCallback.h"

@interface MetalView : MTKView <MTKViewDelegate>
- (void)resize:(CGSize)size;
- (instancetype)initWithFrame:(CGRect)frameRect callback:(MetalViewCallback*)callback
          andDevicePixelRatio:(CGFloat)devicePixelRatio;
- (CAMetalLayer*)metalLayer;
@end