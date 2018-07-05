//
// Created by Semyon Tikhonenko on 7/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import "AppDelegate.h"
#include "VxApp.h"
#import "PortAudio.h"


@implementation AppDelegate {

}

- (instancetype)init {
    self = [super init];
    if (self) {
        PortAudio::init();
        VxApp::instance();
    }

    return self;
}

- (void)dealloc {
    PortAudio::terminate();
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification {

}

@end
