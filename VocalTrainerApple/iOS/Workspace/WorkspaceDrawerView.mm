//
//  WorkspaceDrawerView.m
//  VocalTrainerAppleShared
//
//  Created by Semyon Tykhonenko on 7/27/19.
//

#import "WorkspaceDrawerView.h"
#import <Logic/WorkspaceDrawer.h>
#import <Logic/MetalNvgDrawer.h>
#import <Logic/BaseMouseEventsReceiver.h>
#import <Logic/ProjectControllerBridge.h>

using namespace CppUtils;

@implementation WorkspaceDrawerView {
    WorkspaceDrawer* _workspaceDrawer;
    BaseMouseEventsReceiver* _mouseEventsReceiver;
    ProjectControllerBridge *_projectController;
}

- (instancetype)initWithCoder:(nonnull NSCoder *)coder {
    self = [super initWithCoder:coder];
    if (self) {
        _workspaceDrawer = nullptr;
        _mouseEventsReceiver = new BaseMouseEventsReceiver();
        self.device = MTLCreateSystemDefaultDevice();
        self.delegate = self;
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

        auto* resourcesProvider = new MacOSWorkspaceDrawerResourcesProvider();
        _workspaceDrawer = new WorkspaceDrawer(drawer, _mouseEventsReceiver, resourcesProvider, [] {
            // update workspace drawer automatically, nothing to do here
        });

        if (_projectController != nil) {
            [_projectController setWorkspaceController:_workspaceDrawer];
        }

        return YES;
    }

    return NO;
}

- (void)resizeWorkspaceDrawer:(CGSize)size {
    float devicePixelRatio = float(UIScreen.mainScreen.scale);
    _workspaceDrawer->resize(float(size.width) / devicePixelRatio, float(size.height) / devicePixelRatio,
            devicePixelRatio);
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    if ([self initWorkspaceDrawerIfNeed]) {
        [self resizeWorkspaceDrawer:self.frame.size];
    }

    if (_projectController) {
        _workspaceDrawer->draw();
    }
}

- (void)configure:(ProjectControllerBridge*)projectController {
    _projectController = projectController;
    if (_workspaceDrawer) {
        [_projectController setWorkspaceController:_workspaceDrawer];
    }
}

- (void)dealloc {
    delete _workspaceDrawer;
}


@end
