//
// Created by Semyon Tikhonenko on 10/25/18.
// Copyright (c) 2018 ___FULLUSERNAME___. All rights reserved.
//

#import "MetalView2.h"
#include "MetalNvgDrawer.h"
#include "WorkspaceDrawer.h"

class _PitchesCollector : public PitchesCollector {
public:
    int getPitchesCount() const override {
        return 0;
    }

    float getFrequencyAt(int index) const override {
        return 0;
    }

    double getTimeAt(int index) const override {
        return 0;
    }
};

class _PlayingPitchSequence : public PlayingPitchSequence {
public:
    bool hasPitchNow(const Pitch &pitch) const override {
        return false;
    }

    bool hasAnyPitchNow() const override {
        return false;
    }
};

@implementation MetalView2 {
    Drawer* _drawer;
    WorkspaceDrawer* _workspaceDrawer;
    CGSize _size;
}

- (instancetype)initWithCoder:(nonnull NSCoder *)coder {
    self = [super initWithCoder:coder];
    if (self) {
        self.device = MTLCreateSystemDefaultDevice();
        [self onInit];
    }

    return self;
}

- (instancetype)initWithFrame:(CGRect)frameRect {
    self = [super initWithFrame:frameRect device:MTLCreateSystemDefaultDevice()];
    if (self) {
        [self onInit];
    }

    return self;
}


- (void)onInit {
    self.delegate = self;
    _drawer = nullptr;
    _size = self.frame.size;
}

- (void)initDrawerIfNeed:(MTKView *)view {
    if (!_drawer) {
        _drawer = new MetalNvgDrawer((__bridge void*) view.currentDrawable.layer);
        _drawer->setTextDrawStrategy(Drawer::DONT_DRAW_TEXT);
        _workspaceDrawer = new WorkspaceDrawer(_drawer, [] {

        });
        _workspaceDrawer->resize(_size.width, _size.height, 2);
        _workspaceDrawer->setIntervalWidth(30);
        _workspaceDrawer->setIntervalHeight(11);
        _workspaceDrawer->setFirstVisiblePitch(Pitch("C4"));
        _workspaceDrawer->setPitchesCollector(new _PitchesCollector());
        _workspaceDrawer->setPitchSequence(new _PlayingPitchSequence());
        _workspaceDrawer->setIntervalsPerSecond(3);
        _workspaceDrawer->setRunning(true);
    }
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    [self initDrawerIfNeed:view];
    _workspaceDrawer->resize(size.width / 2, size.height / 2, 2);
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    [self initDrawerIfNeed:view];
    _workspaceDrawer->draw();
}

- (void)resize:(CGSize)size {
    self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, size.width, size.height);
}


@end