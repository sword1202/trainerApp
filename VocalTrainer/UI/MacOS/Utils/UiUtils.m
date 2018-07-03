//
// Created by Semyon Tikhonenko on 7/3/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import "UiUtils.h"


@implementation UiUtils {

}

+ (void)setupFrameEventOfView:(NSView *)view {
    [view setPostsFrameChangedNotifications:YES];

    NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
    [center addObserver:view
               selector:@selector(frameChanged:)
                   name:NSViewFrameDidChangeNotification
                 object:view];
}

@end