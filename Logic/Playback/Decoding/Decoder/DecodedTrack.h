//
// Created by Semyon Tikhonenko on 2018-12-28.
//

#ifndef VOCALTRAINER_DECODEDINSTRUMENTAL_H
#define VOCALTRAINER_DECODEDINSTRUMENTAL_H

#include <string>
#include "WAVFile.h"

struct DecodedTrack {
    std::string rawPcm;
    WavConfig wavConfig;

    void initWithWavData(const std::string &wavData);
};

#endif
