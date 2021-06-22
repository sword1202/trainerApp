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
}

- (instancetype)initWithRecordingsPath:(NSString*)path {
    self = [super init];
    if (self) {
        _cpp = new RecordingsListController(path.UTF8String);
    }

    return self;
}

- (NSInteger)recordingsCount {
    return _cpp->getRecordingsCount();
}

- (RecordingInfo *)getRecording:(NSInteger)index {
    const Recording& recording = _cpp->getRecordingAt(static_cast<int>(index));
    return [[RecordingInfo alloc]
            initWithDate:recording.date / 1000000.0
                filePath:Strings::Utf8ToNSString(recording.filePath)
               songTitle:Strings::Utf8ToNSString(recording.header.songTitleUtf8)
      originalArtistName:Strings::Utf8ToNSString(recording.header.artistNameUtf8)];
}

- (NSArray<NSNumber *> *)getPreviewSamplesForRecordingAt:(NSInteger)index samplesCount:(NSInteger)samplesCount {
    const std::vector<float> &samples = _cpp->getSamplesForRecordingAt(
            static_cast<int>(index), static_cast<int>(samplesCount));
    return Collections::ToPrimitivesNSArray(samples);
}

- (void)deleteRecordingAt:(NSInteger)index {
    _cpp->deleteRecording(static_cast<int>(index));
}


- (void)dealloc {
    delete _cpp;
}

@end