//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import <stdexcept>
#include "AudioPlayer.h"
#include <string>

#import <AVFoundation/AVAudioPlayer.h>

void AudioPlayer::play(const char *audioData, int size, double seek) {
    NSError* error = nil;
    AVAudioPlayer *player = [[AVAudioPlayer alloc] initWithData:[NSData dataWithBytes:audioData
                                                       length:size]
                                  error:&error];
    if (error) {
        std::string message = "Failed to play audio data, errorCode: ";
        message += std::to_string(error.code);
        throw std::runtime_error(message);
    }

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
    [(__bridge AVAudioPlayer*)player stop];
}

AudioPlayer::~AudioPlayer() {
    if (this->player) {
        [(NSObject *)this->player release];
    }
}
