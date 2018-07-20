//
// Created by Semyon Tikhonenko on 7/6/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import "PianoView.h"
#import "PianoDrawer.h"
#include "QuartzDrawer.h"
#import "MainController.h"


@implementation PianoView {
    PianoDrawer* _pianoDrawer;
}

- (instancetype)initWithCoder:(NSCoder *)coder {
    self = [super initWithCoder:coder];
    if (self) {
        _pianoDrawer = nullptr;
    }

    return self;
}


- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    if (!_pianoDrawer) {
        MainController *mainController = MainController::instance();
        _pianoDrawer = new PianoDrawer(new QuartzDrawer(), mainController->getPlayer());

        __weak PianoView* weakSelf = self;
        mainController->setPianoController(_pianoDrawer, [=] {
            [weakSelf setNeedsDisplay:YES];
        });
    }

    _pianoDrawer->draw(self.frame.size.width, self.frame.size.height, 2);
}

- (void)dealloc {
    delete _pianoDrawer;
}


@end