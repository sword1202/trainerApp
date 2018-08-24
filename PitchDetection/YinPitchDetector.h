//
// Created by Semyon Tikhonenko on 9/17/16.
//

#ifndef YINPITCHDETECTION_PITCHDETECTOR_H
#define YINPITCHDETECTION_PITCHDETECTOR_H


#include <cstdint>
#include "PitchDetector.h"

class YinPitchDetector : public PitchDetector {
    int halfBufferSize;		/**< Half the buffer length */
    float* yinBuffer = nullptr;		/**< Buffer that stores the results of the intermediate processing steps of the algorithm */
    float probability;	/**< Probability that the pitch found is correct as a decimal (i.e 0.85 is 85%) */
    float threshold = 0.05;
    int sampleRate;

    void prepare();
    void difference(const int16_t *buffer);
    void cumulativeMeanNormalizedDifference();
    int16_t absoluteThreshold();
    float parabolicInterpolation(int16_t tauEstimate);
    float getPitch(const int16_t *buffer);
public:
    float getFrequencyFromBuffer(const int16_t *buffer) override;

    float getThreshold() const override;
    void setThreshold(float threshold) override;

    bool hasThreshold() const override;

    float getProbability() const;

    void init(int maxBufferSize, int sampleRate) override;
    ~YinPitchDetector();
};


#endif //YINPITCHDETECTION_PITCHDETECTOR_H
