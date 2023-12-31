//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <Logic/Logic-Swift.h>

@protocol ProjectControllerBridgeDelegate;


@interface ProjectControllerBridge : NSObject
- (instancetype)initWithRecordingsPath:(NSString*)recordingsPath;
- (void)addDelegate:(id<ProjectControllerBridgeDelegate>)delegate NS_SWIFT_NAME(add(delegate:));
- (void)removeDelegate:(id<ProjectControllerBridgeDelegate>)delegate NS_SWIFT_NAME(remove(delegate:));

- (void)setWorkspaceController:(void*)workspaceController;

@property (readonly) NSString* artistName;
@property (readonly) NSString* songTitle;

@property (readonly) float maxZoom;
@property (readonly) float minZoom;
@property (nonatomic) float zoom;
- (void)setZoom:(float)zoom intoPoint:(CGPoint)point;
@property (nonatomic) float verticalScrollPosition;

@property (nonatomic) bool lyricsVisible;
@property (readonly) bool tracksVisible;
@property (nonatomic) bool metronomeEnabled;
@property (readonly) bool boundsSelectionEnabled;
@property (readonly) bool isPlaying;
@property (readonly) bool isRecording;
@property (readonly) NSArray<LyricsSection*>* lyricsSections;
@property (readonly) SongTonality* originalTonality;
@property (readonly) double beatsPerMinute;
@property (readonly) double originalBeatsPerMinute;
@property (readonly) double endSeek;
@property (readonly) NSInteger linesCount;

- (bool)hasPlaybackSource NS_SWIFT_NAME(hasPlaybackSource());
- (void)setPlaybackSource:(PlaybackSource*)source NS_SWIFT_NAME(setPlaybackSource(_:));

- (void)goToBeginning;
- (void)toggleTracksVisibility;
- (void)toggleLyricsVisibility;
- (void)toggleMetronomeEnabled;
- (void)toggleBoundsSelectionEnabled;

- (void)togglePlay;
- (void)stop;
- (void)toggleRewindWithBackward:(BOOL)backward;

- (void)setPlaybackProgress:(double)value;
- (void)rewindBackBySeconds:(double)seconds;
- (double)convertSeekToPlaybackProgress:(double)seek;

- (void)setVocalVolume:(float)value;
- (void)setInputSensitivity:(float)value;
- (void)setVocalPianoVolume:(float)value;
- (void)setInstrumentalVolume:(float)value;

- (void)setTempoFactor:(double)value;
- (void)setPitchShift:(NSInteger)value;

- (NSString*)getLineByIndex:(NSInteger)index NS_SWIFT_NAME(getLine(index:));

- (void)setPlaybackBounds:(NSInteger)firstLineIndex lastLineIndex:(NSInteger)lastLineIndex NS_SWIFT_NAME(setPlaybackBounds(firstLineIndex:lastLineIndex:));
- (void)clearPlaybackBounds;
- (BOOL)hasPlaybackBounds;

- (NSArray<NSNumber*>*)getRecordingPreviewSamples:(NSInteger)numberOfSamples NS_SWIFT_NAME(getRecordingPreviewSamples(numberOfSamples:));

- (void)setWorkspaceColorScheme:(WorkspaceColorSchemeBridge *)scheme;
@end
