//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOPLAYER_H
#define VOCALTRAINER_AUDIOPLAYER_H

#ifdef __OBJC__
#import <AVFoundation/AVAudioPlayer.h>
#endif

class AudioPlayer {
#ifdef __OBJC__
    AVAudioPlayer *player;
#endif
public:
    void play(const char* audioData, int size);
};


#endif //VOCALTRAINER_AUDIOPLAYER_H
