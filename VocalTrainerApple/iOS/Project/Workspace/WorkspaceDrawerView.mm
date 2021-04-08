//
//  WorkspaceDrawerView.m
//  VocalTrainerAppleShared
//
//  Created by Semyon Tykhonenko on 7/27/19.
//

#import "WorkspaceDrawerView.h"
#include "iOSWorkspaceDrawerResourcesProvider.h"

@implementation WorkspaceDrawerView {
}

- (WorkspaceDrawerResourcesProvider *)createResourcesProvider {
    return new iOSWorkspaceDrawerResourcesProvider();
}

- (BOOL)drawScrollbars {
    return NO;
}

- (void)workspaceDrawerDidInitialize:(WorkspaceDrawer *)drawer {
    [super workspaceDrawerDidInitialize:drawer];
    drawer->setDrawTracks(false);
    drawer->setMaxZoom(2.5);
}

+ (CGFloat)pianoWidth {
    return WorkspaceDrawer::PIANO_WIDTH;
}

+ (CGFloat)yardstickHeight {
    return WorkspaceDrawer::YARD_STICK_HEIGHT;
}

@end
