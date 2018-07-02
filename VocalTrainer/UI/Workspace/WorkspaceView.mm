//
// Created by Semyon Tikhonenko on 7/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import "WorkspaceView.h"
#include "NvgDrawer.h"


@implementation WorkspaceView {
    std::unique_ptr<NvgDrawer> _drawer;
}

- (instancetype)initWithCoder:(nonnull NSCoder *)coder {
    self = [super initWithCoder:coder];
    if (self) {
        _drawer = nullptr;
        self.delegate = self;
        self.device = MTLCreateSystemDefaultDevice();
    }

    return self;
}


- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size {
    _drawer = std::make_unique<NvgDrawer>((__bridge_retained void *)view.currentDrawable.layer);
}

- (void)drawInMTKView:(MTKView *)view {
    if (!_drawer) {
        _drawer = std::make_unique<NvgDrawer>((__bridge_retained void *)view.currentDrawable.layer);
    }

    CGFloat width = view.frame.size.width;
    CGFloat height = view.frame.size.height;
    _drawer->beginFrame(width, height, 2);
    _drawer->setFillColor({0, 0, 255, 255});
    _drawer->fillRect(0, 0, width, height);
    _drawer->endFrame();
}

@end
