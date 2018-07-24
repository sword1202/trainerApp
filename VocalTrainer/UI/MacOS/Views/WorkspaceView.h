//
// Created by Semyon Tikhonenko on 7/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <MetalKit/MetalKit.h>

@interface WorkspaceView : MTKView<MTKViewDelegate>
- (instancetype)initWithFrame:(CGRect)frameRect;
@end