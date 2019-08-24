//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Logic/Logic-Swift.h>

@protocol ProjectControllerBridgeDelegate;


@interface ProjectControllerBridge : NSObject
- (instancetype)init;
- (void)addDelegate:(id<ProjectControllerBridgeDelegate>)delegate NS_SWIFT_NAME(add(delegate:));
- (void)removeDelegate:(id<ProjectControllerBridgeDelegate>)delegate NS_SWIFT_NAME(remove(delegate:));

#ifdef __cplusplus
- (void)setWorkspaceController:(void*)workspaceController;
#endif

@property (readonly) NSString* artistName;
@property (readonly) NSString* songTitle;

@property (readonly) float maxZoom;
@property (readonly) float minZoom;

@property (readonly) bool lyricsVisible;
@property (readonly) bool tracksVisible;
@property (readonly) bool metronomeEnabled;

- (void)toggleTracksVisibility;
- (void)toggleLyricsVisibility;
- (void)toggleMetronomeEnabled;

- (void)play;
- (void)stop;

- (void)setVocalVolume:(float)value;
- (void)setInputSensitivity:(float)value;
- (void)setVocalPianoVolume:(float)value;
- (void)setInstrumentalVolume:(float)value;
@end