//
// Created by Semyon Tikhonenko on 10/25/18.
// Copyright (c) 2018 ___FULLUSERNAME___. All rights reserved.
//

#import "MetalView.h"

@implementation MetalView {
    CGSize _size;
    MetalViewCallback* _callback;
    CGFloat _devicePixelRatio;
    bool _initialized;
}

- (instancetype)initWithCoder:(nonnull NSCoder *)coder {
    self = [super initWithCoder:coder];
    if (self) {
        self.device = MTLCreateSystemDefaultDevice();
        [self onInit];
    }

    return self;
}

- (instancetype)initWithFrame:(CGRect)frameRect
                     callback:(MetalViewCallback*)callback
          andDevicePixelRatio:(CGFloat)devicePixelRatio {
    self = [super initWithFrame:frameRect device:MTLCreateSystemDefaultDevice()];
    if (self) {
        _devicePixelRatio = devicePixelRatio;
        _callback = callback;
        [self onInit];
    }

    return self;
}

- (CAMetalLayer*)metalLayer {
    return self.currentDrawable.layer;
}

- (void)initMetalIfNeed {
    if (!_initialized) {
        _callback->initMetal();
        _initialized = true;
    }
}

- (void)onInit {
    self.delegate = self;
    _size = self.frame.size;
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    [self initMetalIfNeed];

    int width = int(round(size.width / _devicePixelRatio));
    int height = int(round(size.height / _devicePixelRatio));
    _size = CGSizeMake(width, height);
    _callback->metalResize(width, height);
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    [self initMetalIfNeed];
    _callback->renderMetal(int(_size.width), int(_size.height));
}

- (void)resize:(CGSize)size {
    self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, size.width, size.height);
}


@end