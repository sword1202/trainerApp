//
// Created by Semyon Tikhonenko on 9/17/16.
//

#ifndef PITCHDETECTION_AUDIOUTILS_H
#define PITCHDETECTION_AUDIOUTILS_H


#include <cstdint>

namespace AudioUtils {
    double GetSampleTimeInSeconds(int sampleSize, int sampleRate);
    int64_t GetSampleTimeInMicroseconds(int sampleSize, int sampleRate);

    void MakeLinearFadeInAtBeginning(short *array, int size, int affectedSize);
    void MakeLinearFadeOutAtEnding(short *array, int size, int affectedSize);

    void Int16SamplesIntoFloatSamples(const short* in, int size, float* out);
    void FloatSamplesIntoInt16Samples(const float* in, int size, short* out);
};


#endif //PITCHDETECTION_AUDIOUTILS_H
