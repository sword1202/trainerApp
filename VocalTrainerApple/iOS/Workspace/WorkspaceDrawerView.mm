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

@end
