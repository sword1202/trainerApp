//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#import "ProjectControllerBridge.h"
#include "ProjectController.h"
#include "config.h"
#include <iostream>

using namespace CppUtils;
using std::cout;
using std::endl;

CPP_UTILS_DLLHIDE class DelegateWrapper : public ProjectControllerDelegate {
    NSMutableArray<id<ProjectControllerBridgeDelegate> >* delegates;
public:
    DelegateWrapper() {
        delegates = [NSMutableArray new];
    }

    void addDelegate(id <ProjectControllerBridgeDelegate> delegate) {
        [delegates addObject:delegate];
    }

    void removeDelegate(id <ProjectControllerBridgeDelegate> delegate) {
        [delegates removeObject:delegate];
    }

    void updateAudioLevel(double level) override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerUpdateWithAudioLevel:)]) {
                [delegate projectControllerUpdateWithAudioLevel:level];
            }
        }
    }

    void updateSeek(double seek) override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerUpdateWithSeek:)]) {
                [delegate projectControllerUpdateWithSeek:seek];
            }
        }
    }

    void onPlaybackStarted() override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerPlaybackDidStart)]) {
                [delegate projectControllerPlaybackDidStart];
            }
        }
    }

    void onPlaybackStopped() override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerPlaybackDidStop)]) {
                [delegate projectControllerPlaybackDidStop];
            }
        }
    }

    void updateLyricsLines(const LyricsDisplayedLinesProvider *linesProvider) override {
        if (delegates.count == 0) {
            return;
        }

        int count = linesProvider->getDisplayLinesCount();
        NSMutableArray *lines = [[NSMutableArray alloc] initWithCapacity:static_cast<NSUInteger>(count)];

        for (int i = 0; i < count; ++i) {
            std::u32string_view line = linesProvider->getDisplayedLineAt(i);
            NSString* objCline = Strings::ToNSString(line);
            [lines addObject:objCline];
        }

        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerUpdateWithCurrentLyricsLines:)]) {
                [delegate projectControllerUpdateWithCurrentLyricsLines:lines];
            }
        }
    }

    void updateLyricsSelection(const LyricsPlayer::Selection& selection) override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerUpdateLyricsSelectionWithSelectedCharactersCount:lastCharacterSelectionPosition:lineIndex:)]) {
                [delegate projectControllerUpdateLyricsSelectionWithSelectedCharactersCount:selection.lineSelection.charactersCount
                                                             lastCharacterSelectionPosition:selection.lineSelection.lastCharacterSelectionPosition
                                                                                  lineIndex: selection.lineIndex];
            }
        }
    }

    void updateLyricsVisibilityChanged(bool showLyrics) override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerUpdateWithLyricsVisibility:)]) {
                [delegate projectControllerUpdateWithLyricsVisibility:showLyrics];
            }
        }
    }

    void onTracksVisibilityChanged(bool value) override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerUpdateWithTracksVisibility:)]) {
                [delegate projectControllerUpdateWithTracksVisibility:value];
            }
        }
    }

    void onMetronomeEnabledChanged(bool enabled) override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerWithDidChangeMetronomeEnabled:)]) {
                [delegate projectControllerWithDidChangeMetronomeEnabled:enabled];
            }
        }
    }

    void updateVocalPianoVolume(float volume) override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerUpdateWithVocalPianoVolume:)]) {
                [delegate projectControllerUpdateWithVocalPianoVolume:volume];
            }
        }
    }

    void updateInstrumentalVolume(float volume) override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerUpdateWithInstrumentalVolume:)]) {
                [delegate projectControllerUpdateWithInstrumentalVolume:volume];
            }
        }
    }

    void updateVocalVolume(float volume) override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerUpdateWithVocalVolume:)]) {
                [delegate projectControllerUpdateWithVocalVolume:volume];
            }
        }
    }

    void updateInputSensitivity(float value) override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerUpdateWithInputSensitivity:)]) {
                [delegate projectControllerUpdateWithInputSensitivity:value];
            }
        }
    }

    void updateSaveIndicator(bool hasSaveIndicator) override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerUpdateWithHasSaveIndicator:)]) {
                [delegate projectControllerUpdateWithHasSaveIndicator:hasSaveIndicator];
            }
        }
    }

    void updateZoom(float value) override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerUpdateWithZoom:)]) {
                [delegate projectControllerUpdateWithZoom:value];
            }
        }
    }

    void onRewindStatusChanged(bool rewindRunning, bool backward) override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:
                    @selector(projectControllerWithDidChangeRewindStatus:isBackward:)]) {
                [delegate projectControllerWithDidChangeRewindStatus:rewindRunning isBackward:backward];
            }
        }
    }

    void updateTonality(int shift) override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerUpdateTonalityWithPitchShift:)]) {
                [delegate projectControllerUpdateTonalityWithPitchShift:shift];
            }
        }
    }

    void updateTempoFactor(double factor) override {
        for (id delegate in delegates) {
            if ([delegate respondsToSelector:@selector(projectControllerUpdateTempoWithFactor:)]) {
                [delegate projectControllerUpdateTempoWithFactor:factor];
            }
        }
    }
};

@implementation ProjectControllerBridge {
    ProjectController* _cpp;
    DelegateWrapper* _delegate;
    NSArray* _lyricsSectionsCache;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _delegate = new DelegateWrapper();
        _cpp = new ProjectController(_delegate);
    }

    return self;
}

- (void)addDelegate:(id <ProjectControllerBridgeDelegate>)delegate {
    _delegate->addDelegate(delegate);
    _cpp->updateDelegate();
}

- (void)removeDelegate:(id <ProjectControllerBridgeDelegate>)delegate {
    _delegate->removeDelegate(delegate);
}


- (void)setWorkspaceController:(void*)workspaceController {
    _cpp->setWorkspaceController(static_cast<WorkspaceController *>(workspaceController));
}

- (NSString *)artistName {
    return [NSString stringWithUTF8String:_cpp->getArtistNameUtf8().data()];
}

- (NSString *)songTitle {
    return [NSString stringWithUTF8String:_cpp->getSongTitleUtf8().data()];
}

static LyricsSectionType fromCppToObjCSectionType(Lyrics::SectionType type) {
    switch (type) {
        case Lyrics::CHORUS:
            return LyricsSectionTypeCHORUS;
        case Lyrics::VERSE:
            return LyricsSectionTypeVERSE;
        case Lyrics::BRIDGE:
            return LyricsSectionTypeBRIDGE;
    }
}

- (NSArray<LyricsSection*>*)lyricsSections {
    if (_lyricsSectionsCache) {
        return _lyricsSectionsCache;
    }

    const auto& sections = _cpp->getLyricsSections();
    NSMutableArray *result = [NSMutableArray arrayWithCapacity:sections.size()];
    for (const auto& section : sections) {
        LyricsSectionType type = fromCppToObjCSectionType(section.sectionType);
        LyricsSection *lyricsSection =
                [[LyricsSection alloc]
                        initWithType:type
                              number:section.number
                                seek:section.seek
                      firstLineIndex:section.firstLineIndex
                          linesCount:section.linesCount];
        [result addObject:lyricsSection];
    }

    _lyricsSectionsCache = result;
    return result;
}

- (float)minZoom {
    return _cpp->getMinZoom();
}

- (float)maxZoom {
    return _cpp->getMaxZoom();
}

- (void)setBoundsSelectionEnabled:(BOOL)boundsSelectionEnabled {
    _cpp->setBoundsSelectionEnabled(boundsSelectionEnabled);
}

- (void)togglePlay {
    _cpp->togglePlay();
}

- (void)stop {
    _cpp->stop();
}

- (void)toggleRewindWithBackward:(BOOL)backward {
    _cpp->toggleRewind(backward);
}

- (void)setPlaybackProgress:(double)value {
    _cpp->setPlaybackProgress(value);
}

- (void)rewindBackBySeconds:(double)seconds {
    _cpp->rewindBackBySeconds(seconds);
}


- (double)convertSeekToPlaybackProgress:(double)seek {
    return _cpp->convertSeekToPlaybackProgress(seek);
}


- (void)setVocalVolume:(float)value {
    _cpp->setVocalVolume(value);
}

- (void)setInputSensitivity:(float)value {
    _cpp->setInputSensitivity(value);
}

- (void)setVocalPianoVolume:(float)value {
    _cpp->setVocalPianoVolume(value);
}

- (void)setInstrumentalVolume:(float)value {
    _cpp->setInstrumentalVolume(value);
}

- (void)setTempoFactor:(double)value {
    _cpp->setTempoFactor(value);
}

- (void)setPitchShift:(NSInteger)value {
    _cpp->setPitchShift(static_cast<int>(value));
}

- (NSString *)getLineByIndex:(NSInteger)index {
    return Strings::ToNSString(_cpp->getLyricsLine(static_cast<int>(index)));;
}

- (void)loopLines:(NSInteger)firstLineIndex lastLineIndex:(NSInteger)lastLineIndex {
    _cpp->loopLines(static_cast<int>(firstLineIndex), static_cast<int>(lastLineIndex));
}


- (NSInteger)linesCount {
    return _cpp->getLinesCount();
}

- (void)toggleTracksVisibility {
    _cpp->setTracksVisible(!_cpp->isTracksVisible());
}

- (void)toggleLyricsVisibility {
    _cpp->setLyricsVisible(!_cpp->isLyricsVisible());
}

- (void)toggleMetronomeEnabled {
    _cpp->setMetronomeEnabled(!_cpp->isMetronomeEnabled());
}

- (void)toggleBoundsSelectionEnabled {
    _cpp->setBoundsSelectionEnabled(!_cpp->isBoundsSelectionEnabled());
}


- (float)verticalScrollPosition {
    return _cpp->getVerticalScrollPosition();
}

- (void)setVerticalScrollPosition:(float)position {
    _cpp->setVerticalScrollPosition(position);
}

- (bool)lyricsVisible {
    return _cpp->isLyricsVisible();
}

- (void)setLyricsVisible:(bool)value {
    _cpp->setLyricsVisible(value);
}

- (bool)tracksVisible {
    return _cpp->isTracksVisible();
}
- (bool)metronomeEnabled {
    return _cpp->isMetronomeEnabled();
}

- (void)setMetronomeEnabled:(bool)value {
    _cpp->setMetronomeEnabled(value);
}

- (bool)boundsSelectionEnabled {
    return _cpp->isBoundsSelectionEnabled();
}

- (bool)isPlaying {
    return _cpp->isPlaying();
}

- (void)goToBeginning {
    _cpp->goToBeginning();
}


- (void)dealloc {
    delete _cpp;
}

- (float)zoom {
    return _cpp->getZoom();
}

- (void)setZoom:(float)zoom {
    _cpp->setZoom(zoom);
}

- (void)setZoom:(float)zoom intoPoint:(CGPoint)point {
    _cpp->setZoom(zoom, PointF(point));
}

- (SongTonality*)originalTonality {
    const auto& tonality = _cpp->getOriginalTonality();
    return [[SongTonality alloc] initWithPitchInOctaveIndex:tonality.getPitchInOctaveIndex()
                                                    isMajor:tonality.isMajor()];
}

- (double)beatsPerMinute {
    return _cpp->getBeatsPerMinute();
}

- (double)duration {
    return _cpp->getDuration();
}

- (double)originalBeatsPerMinute {
    return _cpp->getOriginalBeatsPerMinute();
}

@end
