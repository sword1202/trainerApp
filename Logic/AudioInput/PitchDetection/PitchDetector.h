//
// Created by Semyon Tikhonenko on 7/2/18.
//

#ifndef PITCHDETECTION_PITCHDETECTOR_H
#define PITCHDETECTION_PITCHDETECTOR_H


#include <cstdint>

class PitchDetector {
public:
    virtual void init(int maxBufferSize, int sampleRate) = 0;
    virtual float getFrequencyFromBuffer(const int16_t *buffer) = 0;
    virtual ~PitchDetector() = default;
};


#endif //PITCHDETECTION_PITCHDETECTOR_H
