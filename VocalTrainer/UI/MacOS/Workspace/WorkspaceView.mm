//
// Created by Semyon Tikhonenko on 7/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import "WorkspaceView.h"
#include "WorkspaceDrawer.h"
#include "QuartzDrawer.h"
#import "UiUtils.h"
#import "Manager.h"
#import "NvgDrawer.h"


@implementation WorkspaceView {
    std::unique_ptr<WorkspaceDrawer> _workspaceDrawer;
}

- (instancetype)initWithCoder:(nonnull NSCoder *)coder {
    self = [super initWithCoder:coder];
    if (self) {
        [UiUtils setupFrameEventOfView:self];

        [NSTimer scheduledTimerWithTimeInterval:1.0/60.0 target:self
                                       selector:@selector(redraw)
                                       userInfo:nil repeats:YES];
    }

    return self;
}

- (void)redraw {
    [self setNeedsDisplay:YES];
}

- (void)frameChanged:(id)boundsChanged {
    [self resizeDrawer];
    [self redraw];
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];

    const GLubyte *const version = glGetString(GL_VERSION);
    
    if (!_workspaceDrawer) {
        Drawer* drawer = new NvgDrawer();
        _workspaceDrawer = std::make_unique<WorkspaceDrawer>(drawer);
        _workspaceDrawer->setFirstPitchPerfectFrequencyIndex(Pitch("C4").getPerfectFrequencyIndex());
        _workspaceDrawer->setPitchesCollector(Manager::instance()->getPitchInputReader());
        //_workspaceDrawer->setIntervalsPerSecond(3);
        _workspaceDrawer->setIntervalWidth(30);
        _workspaceDrawer->setIntervalHeight(15);
        [self resizeDrawer];
    }

    glViewport(0, 0, dirtyRect.size.width, dirtyRect.size.height);
    glClearColor(1, 1, 1, 1);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    _workspaceDrawer->draw();
    glFinish();
}

- (void)resizeDrawer {
    if (!_workspaceDrawer) {
        return;
    }

    CGFloat width = self.frame.size.width;
    CGFloat height = self.frame.size.height;
    _workspaceDrawer->resize(width, height, 2);
}

@end
