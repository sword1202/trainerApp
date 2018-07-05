//
// Created by Semyon Tikhonenko on 7/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import "WorkspaceView.h"
#include "WorkspaceDrawer.h"
#import "NvgDrawer.h"
#import "VxApp.h"


@implementation WorkspaceView {
    std::unique_ptr<WorkspaceDrawer> _workspaceDrawer;
}

- (instancetype)initWithCoder:(nonnull NSCoder *)coder {
    self = [super initWithCoder:coder];
    if (self) {
        [self onInit];
    }

    return self;
}

- (instancetype)initWithFrame:(CGRect)frameRect {
    self = [super initWithFrame:frameRect device:nil];
    if (self) {
        [self onInit];
    }

    return self;
}


- (void)onInit {
    _workspaceDrawer = nullptr;
    self.delegate = self;
    self.device = MTLCreateSystemDefaultDevice();
}


- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size {
    [self initDrawer];
}

- (void)initDrawer {
    Drawer* drawer = new NvgDrawer((__bridge void *)self.currentDrawable.layer);
    _workspaceDrawer = std::make_unique<WorkspaceDrawer>(drawer);
    VxApp::instance()->setWorkspaceController(_workspaceDrawer.get());
    CGFloat width = self.frame.size.width;
    CGFloat height = self.frame.size.height;
    _workspaceDrawer->resize(width, height, 2);
}

- (void)drawInMTKView:(MTKView *)view {
    if (!_workspaceDrawer) {
        [self initDrawer];
    }

    _workspaceDrawer->draw();
}

@end
