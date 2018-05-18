//
// Created by Semyon Tikhonenko on 8/10/16.
// Copyright (c) 2016 Mac. All rights reserved.
//

#import "StlDebugUtils.h"
#import "PitchGraphView.h"
#include <GLKit/GLKit.h>
#import "GLSceneDrawer.h"
#import "VxFile.h"
#import "MidiFileReader.h"
#import "MvxFile.h"
#import "MvxPlayer.h"
#include "TimeUtils.h"
#import "PortAudio.h"
#include "VxFileAudioPlayer.h"
#include "AudioFilePlayer.h"
#include "Executors.h"

using namespace CppUtils;

@implementation PitchGraphView {
    GLSceneDrawer* _glSceneDrawer;
    AudioPlayer* _player;
    MvxPlayer* _mvxPlayer;
}

- (void)prepareOpenGL {
    PortAudio::init();

    [_window makeFirstResponder:self];
    glClearColor(0, 0, 0, 0);

    [NSTimer scheduledTimerWithTimeInterval:1.0/60.0
                                     target:self
                                   selector:@selector(onTimer:)
                                   userInfo:nil
                                    repeats:YES];
    [self testReadFromMidi];
//    const char* path = "/Users/semyon/Desktop/yo.mvx";
//    std::fstream ee;
    _mvxPlayer = new MvxPlayer("/Users/semyon/Documents/torero.mvx");
    _mvxPlayer->prepare();
    _mvxPlayer->play();
//    _player = AudioFilePlayer::create("/Users/semyon/Downloads/yo.mp3");
//    _player->setPitchShiftInSemiTones(-1);
//    _player->prepare();
//    _player->setSeek(_player->getTrackDurationInSeconds() / 2.0);
//    _player->play();
//    Executors::ExecuteOnMainThreadAfterDelay([=] {
//        _player->setSeek(10.0);
//        }, 5000);
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
    std::cout << "beatsPerMinute = "<<beatsPerMinute<<"\n";
    for (const VxFile& a : vxFiles) {
        std::cout << "Pitches from midi:\n";
        a.writeToStream(std::cout);
        std::cout << "\n";
    }

//    MvxFile mvxFile(vxFiles[0], "/Users/semyon/Documents/toreromidi.mp3", beatsPerMinute);
//    mvxFile.writeToFile("/Users/semyon/Documents/torero.mvx");

//    VxFile vxFile(std::vector<VxPitch> {{Pitch("C4"), 0, 100}, {Pitch("E4"), 100, 100}, {Pitch("A4"), 200, 100}}, 0, 50);

//    std::vector<char> wavAudioData = vxFile.generateWavAudioData(0.5f);
//    VxFile vxFile = vxFiles[0];
//    vxFile.removeSilenceSpaceFromBeginning();
//    _player = new VxFileAudioPlayer(vxFile);
//    _player->prepare();
//    _player->play();
//    _player->play(wavAudioData.data(), wavAudioData.size(), 0);

//    const char* instrumentalPath = [[NSBundle mainBundle] pathForResource:@"yo" ofType:@"mp3"].cString;
//    MvxFile::writeToFile(vxFile, instrumentalPath, "/Users/semyon/Desktop/yo.mvx");
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
    //_glSceneDrawer->draw(dirtyRect.size.width, dirtyRect.size.height);
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
        _player->destroy();
    }
    if (_mvxPlayer) {
        delete _mvxPlayer;
    }

    PortAudio::terminate();
}


@end