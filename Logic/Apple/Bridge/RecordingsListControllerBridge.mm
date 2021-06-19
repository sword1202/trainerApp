//
// Created by Semyon Tikhonenko on 6/19/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#import "RecordingsListControllerBridge.h"
#include "RecordingsListController.h"
#include "StringUtils.h"
#include "AudioUtils.h"
#include "Collections.h"

using namespace CppUtils;

@implementation RecordingsListControllerBridge {
    RecordingsListController* _cpp;
    NSInteger _previewSamplesCount;
    NSMutableDictionary<NSNumber*, RecordingInfo *> * _recordingsCache;
}

- (instancetype)initWithRecordingsPath:(NSString*)path previewSamplesCount:(NSInteger)previewSamplesCount {
    self = [super init];
    if (self) {
        _cpp = new RecordingsListController(path.UTF8String);
        _previewSamplesCount = previewSamplesCount;
        _recordingsCache = [NSMutableDictionary new];
    }

    return self;
}

- (NSInteger)recordingsCount {
    return _cpp->getRecordingsCount();
}

- (RecordingInfo *)getRecording:(NSInteger)index {
    RecordingInfo *cached = _recordingsCache[@(index)];
    if (cached) {
        return cached;
    }

    const Recording& recording = _cpp->getRecordingAt(static_cast<int>(index));
    std::vector<short> previewSamples = AudioUtils::ResizePreviewSamples(
            recording.header.recordingPreviewSamples.data(),
            static_cast<int>(recording.header.recordingPreviewSamples.size()),
            static_cast<int>(_previewSamplesCount));
    RecordingInfo *result = [[RecordingInfo alloc]
            initWithDate:recording.date / 1000000.0
                filePath:Strings::ToNSString(recording.filePath)
               songTitle:Strings::ToNSString(recording.header.songTitleUtf8)
      originalArtistName:Strings::ToNSString(recording.header.artistNameUtf8)
          previewSamples:Collections::ToPrimitivesNSArray(previewSamples)];
    _recordingsCache[@(index)] = result;
    return result;
}

- (void)dealloc {
    delete _cpp;
}


@end