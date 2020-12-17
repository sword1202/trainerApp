//
//  WorkspaceDrawerView.h
//  VocalTrainerAppleShared
//
//  Created by Semyon Tykhonenko on 7/27/19.
//

#import <Foundation/Foundation.h>
#import <MetalKit/MetalKit.h>

@class ProjectControllerBridge;

@interface WorkspaceDrawerView : MTKView <MTKViewDelegate>
- (void)configure:(ProjectControllerBridge*)projectController;
@end
