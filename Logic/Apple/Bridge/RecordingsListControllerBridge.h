//
// Created by Semyon Tikhonenko on 6/19/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Logic/Logic-Swift.h>

@interface RecordingsListControllerBridge : NSObject
- (instancetype)initWithRecordingsPath:(NSString*)path previewSamplesCount:(NSInteger)previewSamplesCount;
- (NSInteger)recordingsCount;
- (RecordingInfo*)getRecording:(NSInteger)index NS_SWIFT_NAME(getRecording(at:));
@end