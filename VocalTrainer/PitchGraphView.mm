//
// Created by Semyon Tikhonenko on 8/10/16.
// Copyright (c) 2016 Mac. All rights reserved.
//

#import "PitchGraphView.h"
#include <GLKit/GLKit.h>
#import "GLSceneDrawer.h"
#import "VxFile.h"
#import "MidiFileReader.h"

@implementation PitchGraphView {
    GLSceneDrawer* _glSceneDrawer;
    AudioPlayer* player;
}

- (void)prepareOpenGL {
    [_window makeFirstResponder:self];
    glClearColor(0, 0, 0, 0);

    [NSTimer scheduledTimerWithTimeInterval:1.0/60.0
                                     target:self
                                   selector:@selector(onTimer:)
                                   userInfo:nil
                                    repeats:YES];
//    const char* path = [[NSBundle mainBundle] pathForResource:@"1" ofType:@"vx"].cString;
//    VxFile vxFile = VxFile::fromFilePath(path);
    MidiFileReader midiFileReader;
    std::vector<VxFile> vxFiles = midiFileReader.read(
            [NSBundle.mainBundle pathForResource:@"melody" ofType:@"mid"].UTF8String);
    for (const VxFile& a : vxFiles) {
        std::cout<<"Pitches from midi:\n";
        a.writeToStream(std::cout);
        std::cout<<"\n";
    }
    const VxFile& vxFile = vxFiles[0];
    std::vector<char> wavAudioData = vxFile.generateWavAudioData();
    player = new AudioPlayer();
    player->play(wavAudioData.data(), wavAudioData.size(), 0);
}

- (id)initWithCoder:(NSCoder *)coder {
    self = [super initWithCoder:coder];
    if (self) {
        _glSceneDrawer = new GLSceneDrawer();
        _glSceneDrawer->setMoveBetweenOctaves(true);
    }

    return self;
}

- (void)onTimer:(id)onTimer {
    [self display];
}

- (void)drawRect:(NSRect)dirtyRect {
    _glSceneDrawer->draw(dirtyRect.size.width, dirtyRect.size.height);
}

- (void)onWavFileSelected:(NSString *)path {
    _glSceneDrawer->readPitchesFromWav(path.UTF8String);
}

- (void)moveBetweenOctavesFlagDidChange:(BOOL)value {
    _glSceneDrawer->setMoveBetweenOctaves(value);
}


- (void)dealloc {
    delete _glSceneDrawer;
    delete player;
}


@end