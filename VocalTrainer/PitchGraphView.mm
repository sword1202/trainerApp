//
// Created by Semyon Tikhonenko on 8/10/16.
// Copyright (c) 2016 Mac. All rights reserved.
//

#import "PitchGraphView.h"
#include <GLKit/GLKit.h>
#import "GLSceneDrawer.h"
#import "VxFile.h"
#import "MidiFileReader.h"
#import "MvxFileWriter.h"
#import "MvxPlayer.h"

@implementation PitchGraphView {
    GLSceneDrawer* _glSceneDrawer;
    AudioPlayer* _player;
    MvxPlayer* _mvxPlayer;
}

- (void)prepareOpenGL {
    [_window makeFirstResponder:self];
    glClearColor(0, 0, 0, 0);

    [NSTimer scheduledTimerWithTimeInterval:1.0/60.0
                                     target:self
                                   selector:@selector(onTimer:)
                                   userInfo:nil
                                    repeats:YES];
    //[self testReadFromMidi];
    const char* path = "/Users/semyon/Desktop/yo.mvx";
    std::fstream ee;
    _mvxPlayer = new MvxPlayer();
    _mvxPlayer->loadFromFile(path);
    _mvxPlayer->play(1.0, 0.5);
}

- (void)testReadFromMidi {
    const char* path = [[NSBundle mainBundle] pathForResource:@"1" ofType:@"vx"].cString;
    //VxFile vxFile = VxFile::fromFilePath(path);
    MidiFileReader midiFileReader;
    std::vector<VxFile> vxFiles;
    double beatsPerMinute;
    midiFileReader.read(
            [NSBundle.mainBundle pathForResource:@"melody" ofType:@"mid"].UTF8String,
            &vxFiles,
            &beatsPerMinute);
    for (const VxFile& a : vxFiles) {
        std::cout << "Pitches from midi:\n";
        a.writeToStream(std::cout);
        std::cout << "\n";
    }
    const VxFile& vxFile = vxFiles[0];
//    std::vector<char> wavAudioData = vxFile.generateWavAudioData(0.5f);
//    _player = new AudioPlayer();
//    _player->play(wavAudioData.data(), wavAudioData.size(), 0);

    const char* instrumentalPath = [[NSBundle mainBundle] pathForResource:@"yo" ofType:@"mp3"].cString;
    MvxFileWriter::writeToFile(vxFile, instrumentalPath, "/Users/semyon/Desktop/yo.mvx");
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
    if (_player) {
        delete _player;
    }
    if (_mvxPlayer) {
        delete _mvxPlayer;
    }
}


@end