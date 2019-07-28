//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol ProjectControllerBridgeDelegate;


@interface ProjectControllerBridge : NSObject
- (instancetype)initWithDelegate:(id <ProjectControllerBridgeDelegate>)delegate;

#ifdef __cplusplus
- (void)setWorkspaceController:(void*)workspaceController;
#endif

@property (readonly) NSString* artistName;
@property (readonly) NSString* songTitle;

- (void)play;
- (void)stop;

- (void)setBoundsSelectionEnabled:(BOOL)boundsSelectionEnabled;
- (void)workspaceMouseDidMove:(float)x;
- (void)workspaceMouseDidClick:(float)x;

- (void)setVocalVolume:(float)value;
- (void)setInputSensitivity:(float)value;
- (void)setVocalPianoVolume:(float)value;
- (void)setInstrumentalVolume:(float)value;
@end