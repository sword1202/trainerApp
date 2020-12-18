//
// Created by Semyon Tykhonenko on 2019-07-28.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#import "InitializationManager.h"
#import <Logic/ApplicationModel.h>


@implementation InitializationManager {

}

+ (void)initializeApplicationIfNeed {
    if (ApplicationModel::instance() == nullptr) {
        ApplicationModel::init();
        auto* player = ApplicationModel::instance()->getPlayer();
        //player->setSource("/Users/semyontykhonenko/Desktop/Ария-Дай руку мне.mvx");
        const char *mvxPath = [NSBundle.mainBundle pathForResource:@"drm" ofType:@"mvx"].UTF8String;
        player->setSource(mvxPath);
        player->prepare();
        player->setPitchShiftInSemiTones(2);
    }
}

@end