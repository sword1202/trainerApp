//
// Created by Semyon Tykhonenko on 2019-07-28.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#import "InitializationManager.h"
#import <Logic/ApplicationModel.h>


@implementation InitializationManager {

}

+ (void)initializeApplicationIfNeed {
    if (!self.isInitialized) {
        ApplicationModel::init();
    }
}

+ (BOOL)isInitialized {
    return ApplicationModel::instance() != nullptr;
}


@end