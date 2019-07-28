//
//  WorkspaceDrawerView.m
//  VocalTrainerAppleShared
//
//  Created by Semyon Tykhonenko on 7/27/19.
//

#import "WorkspaceDrawerView.h"
#import <Logic/WorkspaceDrawer.h>
#import <Logic/MetalNvgDrawer.h>

@implementation WorkspaceDrawerView {
    WorkspaceDrawer* _workspaceDrawer = nullptr;
}

- (instancetype)initWithCoder:(nonnull NSCoder *)coder {
    self = [super initWithCoder:coder];
    if (self) {
        self.device = MTLCreateSystemDefaultDevice();
    }

    return self;
}

- (CAMetalLayer*)metalLayer {
    return self.currentDrawable.layer;
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    if (!_workspaceDrawer) {
        Drawer* drawer = new MetalNvgDrawer((__bridge void *) self.metalLayer);
        _workspaceDrawer = new WorkspaceDrawer(drawer);
    }
}

- (void)resizeWorkspaceDrawer:(CGSize)size {
    _workspaceDrawer->resize(float(size.width), float(size.height),
            float(NSScreen.mainScreen.backingScaleFactor));
}

- (void)initWorkspaceDrawerIfNeedWithSize:(CGSize)size {

}

- (void)drawInMTKView:(nonnull MTKView *)view {

}


@end
