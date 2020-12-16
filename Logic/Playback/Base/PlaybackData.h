//
// Created by Semyon Tikhonenko on 2018-12-27.
//

#ifndef VOCALTRAINER_PLAYBACKDATA_H
#define VOCALTRAINER_PLAYBACKDATA_H

#include "WAVFile.h"
#include "AudioStreamDescription.h"

struct PlaybackData : AudioStreamDescription {
    double totalDurationInSeconds = -1;

    PlaybackData() = default;
    PlaybackData(const WavConfig &wavConfig, int samplesPerBuffer);
};


#endif //VOCALTRAINER_PLAYBACKDATA_H
