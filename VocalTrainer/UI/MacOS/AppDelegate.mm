//
// Created by Semyon Tikhonenko on 7/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import "AppDelegate.h"
#include "Manager.h"


@implementation AppDelegate {

}

- (instancetype)init {
    self = [super init];
    if (self) {
        Manager::init();
        Manager::instance()->getPitchInputReader()->start();
    }

    return self;
}


- (void)applicationDidFinishLaunching:(NSNotification *)notification {

}

@end
