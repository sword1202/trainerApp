//
// Created by Semyon Tikhonenko on 7/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import "WorkspaceView.h"
#include "NvgDrawer.h"


@implementation WorkspaceView {
    std::unique_ptr<NvgDrawer> _drawer;
}

- (instancetype)initWithFrame:(CGRect)frameRect {
    self = [super initWithFrame:frameRect device:MTLCreateSystemDefaultDevice()];
    if (self) {
        _drawer = nullptr;
        self.delegate = self;
    }

    return self;
}


- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size {
    if (!_drawer) {
        _drawer = std::make_unique<MetalNvgDrawer>((__bridge void *)view.currentDrawable.layer);
    }
}

- (void)drawInMTKView:(MTKView *)view {
    if (!_drawer) {
        _drawer = std::make_unique<MetalNvgDrawer>((__bridge void *)view.currentDrawable.layer);
    }

    CGFloat width = view.frame.size.width;
    CGFloat height = view.frame.size.height;
    _drawer->beginFrame(width, height, 2);
    _drawer->setFillColor({0, 0, 255, 255});
    _drawer->fillRect(0, 0, width, height);
    _drawer->endFrame();
}

@end
