//
// Created by Semyon Tikhonenko on 9/17/16.
//

#ifndef PITCHDETECTION_WAVFILEPITCHESREADER_H
#define PITCHDETECTION_WAVFILEPITCHESREADER_H

#include <cstdint>
#include <vector>
#include "Pitch.h"
#include "WAVFile.h"
#include "YinPitchDetector.h"

struct PitchDetection {
    int64_t durationInMicroSeconds;
    Pitch pitch;
};

class WavFilePitchesReader {
    YinPitchDetector pitchDetector;
public:
    WavFilePitchesReader();
    std::vector<PitchDetection> readPitchesFromWavFile(std::istream* stream, float threshold);
};


#endif //PITCHDETECTION_WAVFILEPITCHESREADER_H
