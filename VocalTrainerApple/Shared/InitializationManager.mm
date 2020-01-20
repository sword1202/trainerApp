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
        PortAudio::init();
        ApplicationModel::init(new ApplicationModel());
        auto* player = ApplicationModel::instance()->getPlayer();
        player->setSource("/Users/semyontykhonenko/Desktop/Vocalis1.vx");
        player->prepare();
    }
}

@end