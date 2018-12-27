//
// Created by Semyon Tikhonenko on 2018-12-27.
//

#ifndef VOCALTRAINER_PLAYBACKDATA_H
#define VOCALTRAINER_PLAYBACKDATA_H

#include "PortAudioUtils.h"

struct PlaybackData {
    int sampleRate = -1;
    int framesPerBuffer = -1;
    int numChannels = -1;
    double totalDurationInSeconds = -1;
    PaSampleFormat format = paInt16;
};


#endif //VOCALTRAINER_PLAYBACKDATA_H
