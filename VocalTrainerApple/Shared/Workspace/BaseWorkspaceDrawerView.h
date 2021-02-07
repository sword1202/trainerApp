//
// Created by Semyon Tikhonenko on 12/17/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MetalKit/MetalKit.h>
#ifdef  __cplusplus
#include <Logic/WorkspaceDrawerResourcesProvider.h>
#include <Logic/BaseMouseEventsReceiver.h>
#include <Logic/WorkspaceDrawer.h>
#endif

@class ProjectControllerBridge;

@interface BaseWorkspaceDrawerView : MTKView <MTKViewDelegate>
@property void(^onWorkspaceControllerChanged)();
- (void*)workspaceController;
- (BOOL)drawScrollbars;
- (void)scrollBy:(CGPoint)distance;
#ifdef  __cplusplus
- (void)workspaceDrawerDidInitialize:(WorkspaceDrawer*)drawer;
- (WorkspaceDrawerResourcesProvider*)createResourcesProvider;
- (BaseMouseEventsReceiver*)mouseEventsReceiver;
#endif
@end
