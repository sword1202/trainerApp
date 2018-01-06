//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import <stdexcept>
#include "AudioPlayer.h"
#include <string>

void AudioPlayer::play(const char *audioData, int size) {
    NSError* error = nil;
    player = [[AVAudioPlayer alloc] initWithData:[NSData dataWithBytes:audioData
                                                       length:size]
                                  error:&error];
    if (error) {
        std::string message = "Failed to play audio data, errorCode: ";
        message += std::to_string(error.code);
        throw std::runtime_error(message);
    }

    [player play];
}
