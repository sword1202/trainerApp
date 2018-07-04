//
// Created by Semyon Tikhonenko on 7/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import "WorkspaceView.h"
#include "WorkspaceDrawer.h"
#import "NvgDrawer.h"
#import "Manager.h"


@implementation WorkspaceView {
    std::unique_ptr<WorkspaceDrawer> _workspaceDrawer;
}

- (instancetype)initWithCoder:(nonnull NSCoder *)coder {
    self = [super initWithCoder:coder];
    if (self) {
        _workspaceDrawer = nullptr;
        self.delegate = self;
        self.device = MTLCreateSystemDefaultDevice();
    }

    return self;
}


- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size {
    [self initDrawer:view];
}

- (void)initDrawer:(MTKView *)view {
    Drawer* drawer = new NvgDrawer((__bridge void *)view.currentDrawable.layer);
    _workspaceDrawer = std::make_unique<WorkspaceDrawer>(drawer);
    CGFloat width = view.frame.size.width;
    CGFloat height = view.frame.size.height;
    _workspaceDrawer->resize(width, height, 2);
    _workspaceDrawer->setIntervalWidth(30);
    _workspaceDrawer->setIntervalHeight(15);
    _workspaceDrawer->setPitchesCollector(Manager::instance()->getPitchInputReader());
}

- (void)drawInMTKView:(MTKView *)view {
    if (!_workspaceDrawer) {
        [self initDrawer:view];
    }

    _workspaceDrawer->draw();
}

@end
