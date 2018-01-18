//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import <stdexcept>
#include "AudioPlayer.h"
#include <string>
#include "TimeUtils.h"

#import <AVFoundation/AVAudioPlayer.h>
#import <iostream>

using namespace CppUtils;
using std::cout;

void AudioPlayer::play(const char *audioData, int size, double seek) {
    NSError* error = nil;
    int64_t i = TimeUtils::NowInMicroseconds();
    AVAudioPlayer *player = [[AVAudioPlayer alloc] initWithData:[NSData dataWithBytes:audioData
                                                       length:size]
                                  error:&error];
    if (error) {
        std::string message = "Failed to play audio data, errorCode: ";
        message += std::to_string(error.code);
        throw std::runtime_error(message);
    }

    player.currentTime = seek;
    [player play];
    if (this->player) {
        [(NSObject *)this->player release];
    }

    this->player = (__bridge void*)player;
}

bool AudioPlayer::isPlaying() {
    return player && [(__bridge AVAudioPlayer*)player isPlaying];
}

void AudioPlayer::stop() {
    if (this->player) {
        [(NSObject *)this->player release];
        [(__bridge AVAudioPlayer*)player stop];
        this->player = 0;
    }
}

AudioPlayer::~AudioPlayer() {
    if (this->player) {
        [(NSObject *)this->player release];
    }
}

void AudioPlayer::pause() {
    [(__bridge AVAudioPlayer*)player pause];
}

void AudioPlayer::resume() {
    [(__bridge AVAudioPlayer*)player play];
}

void AudioPlayer::seek(double timeStamp) {
    ((__bridge AVAudioPlayer*)player).currentTime = timeStamp;
}
