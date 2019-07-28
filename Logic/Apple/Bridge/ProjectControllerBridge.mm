//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#import "ProjectControllerBridge.h"
#include "ProjectController.h"
#include "config.h"
#import <Logic/Logic-Swift.h>

CPP_UTILS_DLLHIDE class DelegateWrapper : public ProjectControllerDelegate {
    id <ProjectControllerBridgeDelegate> delegate;
public:
    DelegateWrapper(id <ProjectControllerBridgeDelegate> delegate) : delegate(delegate) {

    }

    void updateAudioLevel(double level) override {
        [delegate projectControllerUpdateWithAudioLevel:level];
    }

    void updateSeek(double seek) override {
        [delegate projectControllerUpdateWithSeek:seek];
    }

    void onPlaybackStarted() override {
        [delegate projectControllerPlaybackDidStart];
    }

    void onPlaybackStopped() override {
        [delegate projectControllerPlaybackDidStop];
    }

    void onHasLyricsChanged(bool hasLyrics) override {
        [delegate projectControllerWithDidChangeHasLyrics:hasLyrics];
    }

    void updateLyricsLine(const std::string &lyricsLineUtf8) override {
        [delegate projectControllerUpdateWithLyricsLine:[NSString stringWithUTF8String:lyricsLineUtf8.data()]];
    }

    void onShowLyricsChanged(bool showLyrics) override {
        [delegate projectControllerWithDidChangeShowLyrics:showLyrics];
    }

    void onMetronomeEnabledChanged(bool enabled) override {
        [delegate projectControllerWithDidChangeMetronomeEnabled:enabled];
    }

    void updateVocalPianoVolume(float volume) override {
        [delegate projectControllerUpdateWithVocalPianoVolume:volume];
    }

    void updateInstrumentalVolume(float volume) override {
        [delegate projectControllerUpdateWithInstrumentalVolume:volume];
    }

    void updateVocalVolume(float volume) override {
        [delegate projectControllerUpdateWithVocalVolume:volume];
    }

    void updateInputSensitivity(float value) override {
        [delegate projectControllerUpdateWithInputSensitivity:value];
    }

    void updateSaveIndicator(bool hasSaveIndicator) override {
        [delegate projectControllerUpdateWithHasSaveIndicator:hasSaveIndicator];
    }
};

@implementation ProjectControllerBridge {
    ProjectController* _cpp;
    DelegateWrapper* _delegate;
}

- (instancetype)initWithDelegate:(id <ProjectControllerBridgeDelegate>)delegate {
    self = [super init];
    if (self) {
        _delegate = new DelegateWrapper(delegate);
        _cpp = new ProjectController(_delegate);
    }

    return self;
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

- (void)play {
    _cpp->play();
}

- (void)stop {
    _cpp->stop();
}

- (void)setBoundsSelectionEnabled:(BOOL)boundsSelectionEnabled {
    _cpp->setBoundsSelectionEnabled(boundsSelectionEnabled);
}

- (void)workspaceMouseDidMove:(float)x {
    _cpp->onWorkspaceMouseMove(x);
}

- (void)workspaceMouseDidClick:(float)x {
    _cpp->onWorkspaceMouseClick(x);
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


- (void)dealloc {
    delete _cpp;
}

@end
