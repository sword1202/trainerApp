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
        ApplicationModel::init(new ApplicationModel());
        auto* player = ApplicationModel::instance()->getPlayer();
        //player->setSource("/Users/semyontykhonenko/Desktop/Ария-Дай руку мне.mvx");
        player->setSource("/Users/semyon/drm.mvx");
        player->prepare();
        player->setPitchShiftInSemiTones(2);
    }
}

@end