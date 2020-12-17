//
//  WorkspaceDrawerView.m
//  VocalTrainerAppleShared
//
//  Created by Semyon Tykhonenko on 7/27/19.
//

#import "WorkspaceDrawerView.h"
#import "MacOSWorkspaceDrawerResourcesProvider.h"

using namespace CppUtils;

@implementation WorkspaceDrawerView {
    NSTrackingArea *_trackingArea;
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

- (void)mouseDown:(NSEvent *)event {
    self.mouseEventsReceiver->setLeftMouseDown(true);
    [self updateMouseLocation:event];
}

- (void)mouseUp:(NSEvent *)event {
    self.mouseEventsReceiver->setLeftMouseDown(false);
    [self updateMouseLocation:event];
}


- (void)rightMouseDown:(NSEvent *)event {
    self.mouseEventsReceiver->setRightMouseDown(true);
    [self updateMouseLocation:event];
}

- (void)rightMouseUp:(NSEvent *)event {
    self.mouseEventsReceiver->setRightMouseDown(false);
    [self updateMouseLocation:event];
}

- (void)mouseMoved:(NSEvent *)event {
    [self updateMouseLocation:event];
}

- (void)mouseDragged:(NSEvent *)event {
    [self updateMouseLocation:event];
}

- (void)rightMouseDragged:(NSEvent *)event {
    [self updateMouseLocation:event];
}

- (void)updateMouseLocation:(NSEvent *)event {
    NSPoint mouseLocation = [self convertPoint:[event locationInWindow] fromView:nil];
    mouseLocation.y = self.frame.size.height - mouseLocation.y;
    self.mouseEventsReceiver->setCurrentMousePosition(PointF(float(mouseLocation.x), float(mouseLocation.y)));
}

- (WorkspaceDrawerResourcesProvider *)createResourcesProvider {
    return new MacOSWorkspaceDrawerResourcesProvider();
}


@end
