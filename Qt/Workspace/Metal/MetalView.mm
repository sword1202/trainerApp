//
// Created by Semyon Tikhonenko on 7/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import "MetalView.h"
#include "NvgDrawer.h"
#include <QThread>
#include <QApplication>

#define LOCK std::lock_guard<std::mutex> _(_sizeMutex)

CVReturn renderCallback(CVDisplayLinkRef displayLink,
                                      const CVTimeStamp *inNow,
                                      const CVTimeStamp *inOutputTime,
                                      CVOptionFlags flagsIn,
                                      CVOptionFlags *flagsOut,
                                      void *displayLinkContext)
{
    [(__bridge MetalView*)displayLinkContext render];
    return kCVReturnSuccess;
}

@implementation MetalView {
    CGSize _size;
    std::mutex _sizeMutex;
    MetalViewCallback* _callback;
    bool _sizeChanged;
    bool _initialized;

    CAMetalLayer *_metalLayer;
    id<MTLDevice> _device;
    CVDisplayLinkRef _displayLink;
}

- (instancetype)initWithFrame:(CGRect)frameRect callback:(MetalViewCallback*)callback {
    self = [super initWithFrame:frameRect];
    if (self) {
        _sizeChanged = true;
        _initialized = false;
        _size = self.frame.size;
        _callback = callback;

        _metalLayer = [CAMetalLayer new];
        _device = MTLCreateSystemDefaultDevice();
        _metalLayer.device = _device;
        _metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        _metalLayer.framebufferOnly = false;
        _metalLayer.frame = self.layer.frame;
        [self setLayer:_metalLayer];
        [self setWantsLayer:YES];

        CGDirectDisplayID   displayID = CGMainDisplayID();
        CVReturn            error2 = kCVReturnSuccess;
        error2 = CVDisplayLinkCreateWithCGDisplay(displayID, &_displayLink);
        if (error2)
        {
            NSLog(@"DisplayLink created with error:%d", error2);
        }

        CVDisplayLinkSetOutputCallback(_displayLink, renderCallback, self);
        CVDisplayLinkStart(_displayLink);
    }

    return self;
}

- (void)render {
    if (!_initialized) {
        _callback->initMetal();
        _initialized = true;
    }

    CGSize size;
    bool sizeChanged;
    {
        LOCK;
        size  = _size;
        sizeChanged = _sizeChanged;
    }
    if (sizeChanged) {
        _callback->metalResize((int)size.width, (int)size.height);
        LOCK;
        _sizeChanged = false;
    }
    _callback->renderMetal((int)size.width, (int)size.height);
}

- (void)resize:(CGSize)size {
    self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, size.width, size.height);
    LOCK;
    _size = size;
    _sizeChanged = true;
}

- (CAMetalLayer*)metalLayer {
    return _metalLayer;
}

@end
