//
// Created by Semyon Tikhonenko on 12/17/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#import "BaseWorkspaceDrawerView.h"
#import <Logic/WorkspaceDrawer.h>
#import <Logic/MetalNvgDrawer.h>
#include "DefineAppleConditionals.h"

using namespace CppUtils;

@implementation BaseWorkspaceDrawerView {
    WorkspaceDrawer* _workspaceDrawer;
    BaseMouseEventsReceiver* _mouseEventsReceiver;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        [self internalInit];
    }

    return self;
}

- (void)internalInit {
    _workspaceDrawer = nullptr;
    _mouseEventsReceiver = new BaseMouseEventsReceiver();
    self.device = MTLCreateSystemDefaultDevice();
    self.delegate = self;
}

- (instancetype)initWithCoder:(nonnull NSCoder *)coder {
    self = [super initWithCoder:coder];
    if (self) {
        [self internalInit];
    }

    return self;
}


- (CAMetalLayer*)metalLayer {
    return self.currentDrawable.layer;
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    [self initWorkspaceDrawerIfNeed];
    [self resizeWorkspaceDrawer:size];
}

- (BOOL)initWorkspaceDrawerIfNeed {
    if (!_workspaceDrawer) {
        Drawer* drawer = new MetalNvgDrawer((__bridge void *) self.metalLayer);

        WorkspaceDrawerResourcesProvider* resourcesProvider = [self createResourcesProvider];
        assert(resourcesProvider && "Override createResourcesProvider");
        _workspaceDrawer = new WorkspaceDrawer(
                drawer, _mouseEventsReceiver, resourcesProvider, self.drawScrollbars, [] {
            // update workspace drawer automatically, nothing to do here
        });
        self.onWorkspaceControllerChanged();
        [self workspaceDrawerDidInitialize:_workspaceDrawer];

        return YES;
    }

    return NO;
}

- (void)resizeWorkspaceDrawer:(CGSize)size {
    float devicePixelRatio =
#ifdef HAS_UIKIT
            float(UIScreen.mainScreen.scale);
#else
            float(NSScreen.mainScreen.backingScaleFactor);
#endif
    _workspaceDrawer->resize(float(size.width) / devicePixelRatio, float(size.height) / devicePixelRatio,
            devicePixelRatio);
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    if ([self initWorkspaceDrawerIfNeed]) {
        [self resizeWorkspaceDrawer:self.frame.size];
    }

    if (_workspaceDrawer) {
        _workspaceDrawer->draw();
    }
}

- (void *)workspaceController {
    return _workspaceDrawer;
}

- (BOOL)drawScrollbars {
    return YES;
}

- (void)scrollBy:(CGPoint)distance {
    _workspaceDrawer->scrollBy(static_cast<float>(distance.x), static_cast<float>(distance.y));
}


- (void)workspaceDrawerDidInitialize:(WorkspaceDrawer *)drawer {

}

- (WorkspaceDrawerResourcesProvider*)createResourcesProvider {
    return nullptr;
}

- (BaseMouseEventsReceiver *)mouseEventsReceiver {
    return _mouseEventsReceiver;
}

- (void)dealloc {
    delete _workspaceDrawer;
}


@end