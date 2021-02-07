//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#import "ProjectControllerBridge.h"
#include "ProjectController.h"
#include "config.h"

using namespace CppUtils;

CPP_UTILS_DLLHIDE class DelegateWrapper : public ProjectControllerDelegate {
    std::vector<__weak id<ProjectControllerBridgeDelegate> > delegates;
public:
    DelegateWrapper(){
    }

    void addDelegate(id <ProjectControllerBridgeDelegate> delegate) {
        delegates.push_back(delegate);
    }

    void removeDelegate(id <ProjectControllerBridgeDelegate> delegate) {
        CppUtils::Remove(delegates, delegate);
    }

    void updateAudioLevel(double level) override {
        for (auto delegate : delegates) {
            [delegate projectControllerUpdateWithAudioLevel:level];
        }
    }

    void updateSeek(double seek) override {
        for (auto delegate : delegates) {
            [delegate projectControllerUpdateWithSeek:seek];
        }
    }

    void onPlaybackStarted() override {
        for (auto delegate : delegates) {
            [delegate projectControllerPlaybackDidStart];
        }
    }

    void onPlaybackStopped() override {
        for (auto delegate : delegates) {
            [delegate projectControllerPlaybackDidStop];
        }
    }

    void onHasLyricsChanged(bool hasLyrics) override {
        for (auto delegate : delegates) {
            [delegate projectControllerWithDidChangeHasLyrics:hasLyrics];
        }
    }

    void updateLyricsText(const std::string &lyricsLineUtf8) override {
        for (auto delegate : delegates) {
            [delegate projectControllerUpdateWithLyricsText:[NSString stringWithUTF8String:lyricsLineUtf8.data()]];
        }
    }

    void onLyricsVisibilityChanged(bool showLyrics) override {
        for (auto delegate : delegates) {
            [delegate projectControllerWithDidChangeLyricsVisibility:showLyrics];
        }
    }

    void onTracksVisibilityChanged(bool value) override {
        for (auto delegate : delegates) {
            [delegate projectControllerWithDidChangeTracksVisibility:value];
        }
    }

    void onMetronomeEnabledChanged(bool enabled) override {
        for (auto delegate : delegates) {
            [delegate projectControllerWithDidChangeMetronomeEnabled:enabled];
        }
    }

    void updateVocalPianoVolume(float volume) override {
        for (auto delegate : delegates) {
            [delegate projectControllerUpdateWithVocalPianoVolume:volume];
        }
    }

    void updateInstrumentalVolume(float volume) override {
        for (auto delegate : delegates) {
            [delegate projectControllerUpdateWithInstrumentalVolume:volume];
        }
    }

    void updateVocalVolume(float volume) override {
        for (auto delegate : delegates) {
            [delegate projectControllerUpdateWithVocalVolume:volume];
        }
    }

    void updateInputSensitivity(float value) override {
        for (auto delegate : delegates) {
            [delegate projectControllerUpdateWithInputSensitivity:value];
        }
    }

    void updateSaveIndicator(bool hasSaveIndicator) override {
        for (auto delegate : delegates) {
            [delegate projectControllerUpdateWithHasSaveIndicator:hasSaveIndicator];
        }
    }

    void onZoomChanged(float value) override {
        for (auto delegate : delegates) {
            [delegate projectControllerWithDidChangeZoom:value];
        }
    }

    void onRewindStatusChanged(bool rewindRunning, bool backward) override {
        for (auto delegate : delegates) {
            [delegate projectControllerWithDidChangeRewindStatus:rewindRunning isBackward:backward];
        }
    }
};

@implementation ProjectControllerBridge {
    ProjectController* _cpp;
    DelegateWrapper* _delegate;
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
- (bool)tracksVisible {
    return _cpp->isTracksVisible();
}
- (bool)metronomeEnabled {
    return _cpp->isMetronomeEnabled();
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


@end
