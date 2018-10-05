//
// Created by Semyon Tikhonenko on 9/17/16.
//

#include <cmath>
#include <assert.h>
#include "AudioUtils.h"

namespace AudioUtils {
    int64_t GetSampleTimeInMicroseconds(int sampleSize, int sampleRate) {
        return int16_t(round(1000000.0 * sampleSize / (double)sampleRate));
    }

    double GetSampleTimeInSeconds(int sampleSize, int sampleRate) {
        return (double)sampleSize / (double)sampleRate;
    }

    void MakeLinearFadeInAtBeginning(short *array, int size, int affectedSize) {
        assert(affectedSize <= size);
        double volumePercentageIncrement = 1.0 / affectedSize;
        double multiplier = 0.0;
        for (int i = 0; i < affectedSize; ++i) {
            multiplier += volumePercentageIncrement;
            array[i] *= multiplier;
        }
    }

    void MakeLinearFadeOutAtEnding(short *array, int size, int affectedSize) {
        assert(affectedSize <= size);
        double volumePercentageIncrement = 1.0 / affectedSize;
        double multiplier = 0.0;
        for (int i = 0; i < affectedSize; ++i) {
            multiplier += volumePercentageIncrement;
            array[size - i] *= multiplier;
        }
    }

    void Int16SamplesIntoFloatSamples(const short* in, int size, float* out) {
        const float div = 1.0f/32768.0f;

        for (int i = 0; i < size; ++i) {
            out[i] = in[i] * div;
        }
    }

    void FloatSamplesIntoInt16Samples(const float* in, int size, short* out) {
        const float div = 32768.0f;
        for (int i = 0; i < size; ++i) {
            out[i] = static_cast<short>(in[i] * div);
        }
    }
};