//
// Created by Semyon Tikhonenko on 2018-12-27.
//

#ifndef VOCALTRAINER_TONALITYCHANGESAPPLIER_H
#define VOCALTRAINER_TONALITYCHANGESAPPLIER_H

#include <SoundTouch/SoundTouch.h>
#include <vector>
#include "PlaybackData.h"

class SoundTouchManager {
    soundtouch::SoundTouch soundTouch;
    std::vector<float> tempFloatBuffer;
    PlaybackData playbackData;
public:
    void onPlayBackDataReceived(const PlaybackData& playbackData);
    void changeTonality(int16_t* data, int framesCount, int shiftInSemiTones);
};


#endif //VOCALTRAINER_TONALITYCHANGESAPPLIER_H
