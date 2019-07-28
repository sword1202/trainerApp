//
//  WorkspaceDrawerView.m
//  VocalTrainerAppleShared
//
//  Created by Semyon Tykhonenko on 7/27/19.
//

#import "WorkspaceDrawerView.h"
#import "MacOSWorkspaceDrawerSetupDelegate.h"
#import <Logic/WorkspaceDrawer.h>
#import <Logic/MetalNvgDrawer.h>
#import <Logic/BaseMouseEventsReceiver.h>
#import <Logic/ProjectControllerBridge.h>
#import <Logic/ProjectControllerBridge.h>

using namespace CppUtils;

@implementation WorkspaceDrawerView {
    WorkspaceDrawer* _workspaceDrawer;
    BaseMouseEventsReceiver* _mouseEventsReceiver;
    NSTrackingArea *_trackingArea;
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

- (void)updateTrackingAreas {
    [super updateTrackingAreas];

    if (_trackingArea != nil) {
        [self removeTrackingArea:_trackingArea];
    }

    NSTrackingAreaOptions options = (NSTrackingActiveAlways | NSTrackingInVisibleRect |
            NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved);

    _trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds] options:options owner:self userInfo:nil];
    [self addTrackingArea:_trackingArea];
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

#ifdef TARGET_OS_MAC
        MacOSWorkspaceDrawerSetupDelegate imagesFactory;
#else
#error Define imagesFactory
#endif
        _workspaceDrawer = new WorkspaceDrawer(drawer, _mouseEventsReceiver, imagesFactory, [] {
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
    float devicePixelRatio = float(NSScreen.mainScreen.backingScaleFactor);
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

- (void)mouseDown:(NSEvent *)event {
    _mouseEventsReceiver->setLeftMouseDown(true);
    [self updateMouseLocation:event];
}

- (void)mouseUp:(NSEvent *)event {
    _mouseEventsReceiver->setLeftMouseDown(false);
    [self updateMouseLocation:event];
}


- (void)rightMouseDown:(NSEvent *)event {
    _mouseEventsReceiver->setRightMouseDown(true);
    [self updateMouseLocation:event];
}

- (void)rightMouseUp:(NSEvent *)event {
    _mouseEventsReceiver->setRightMouseDown(false);
    [self updateMouseLocation:event];
}

- (void)mouseMoved:(NSEvent *)event {
    [self updateMouseLocation:event];
}

- (void)updateMouseLocation:(NSEvent *)event {
    NSPoint mouseLocation = [self convertPoint:[event locationInWindow] fromView:nil];
    _mouseEventsReceiver->setCurrentMousePosition(PointF(float(mouseLocation.x), float(mouseLocation.y)));
}

- (void)configure:(ProjectControllerBridge*)projectController {
    _projectController = projectController;
    if (_workspaceDrawer) {
        [_projectController setWorkspaceController:_workspaceDrawer];
    }
}

- (void)dealloc {
    delete _workspaceDrawer;
    delete _mouseEventsReceiver;
}


@end
