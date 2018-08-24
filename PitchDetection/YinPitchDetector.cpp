//
// Created by Semyon Tikhonenko on 9/17/16.
//

#include <cstring>
#include <cstdlib>
#include <cassert>
#include "YinPitchDetector.h"

void YinPitchDetector::difference(const int16_t *buffer) {
    int16_t i;
    int16_t tau;
    float delta;

    /* Calculate the difference for difference shift values (tau) for the half of the samples */
    for(tau = 0 ; tau < halfBufferSize; tau++){

        /* Take the difference of the signal with a shifted version of itself, then square it.
         * (This is the Yin algorithm's tweak on autocorellation) */
        for(i = 0; i < halfBufferSize; i++){
            delta = buffer[i] - buffer[i + tau];
            yinBuffer[tau] += delta * delta;
        }
    }
}

void YinPitchDetector::prepare() {
    probability = 0.0f;
    memset(yinBuffer, 0, halfBufferSize * sizeof(float));
}

void YinPitchDetector::cumulativeMeanNormalizedDifference() {
    int16_t tau;
    float runningSum = 0;
    yinBuffer[0] = 1;

    /* Sum all the values in the autocorellation resultBuffer and nomalise the result, replacing
     * the value in the autocorellation resultBuffer with a cumulative mean of the normalised difference */
    for (tau = 1; tau < halfBufferSize; tau++) {
        runningSum += yinBuffer[tau];
        yinBuffer[tau] *= tau / runningSum;
    }
}

float YinPitchDetector::getPitch(const int16_t *buffer) {
    int16_t tauEstimate = -1;
    float pitchInHertz = -1;

    /* Step 1: Calculates the squared difference of the signal with a shifted version of itself. */
    difference(buffer);

    /* Step 2: Calculate the cumulative mean on the normalised difference calculated in step 1 */
    cumulativeMeanNormalizedDifference();

    /* Step 3: Search through the normalised cumulative mean array and find values that are over the threshold */
    tauEstimate = absoluteThreshold();

    /* Step 5: Interpolate the shift value (tau) to improve the lastDetectedPitch estimate. */
    if(tauEstimate != -1){
        pitchInHertz = sampleRate / parabolicInterpolation(tauEstimate);
    }

    return pitchInHertz;
}

int16_t YinPitchDetector::absoluteThreshold() {
    int16_t tau;

    /* Search through the array of cumulative mean values, and look for ones that are over the threshold
     * The first two positions in yinBuffer are always so start at the third (index 2) */
    for (tau = 2; tau < halfBufferSize ; tau++) {
        if (yinBuffer[tau] < threshold) {
            while (tau + 1 < halfBufferSize && yinBuffer[tau + 1] < yinBuffer[tau]) {
                tau++;
            }
            /* found tau, exit loop and return
             * store the probability
             * From the YIN paper: The yin->threshold determines the list of
             * candidates admitted to the set, and can be interpreted as the
             * proportion of aperiodic power tolerated
             * within a periodic signal.
             *
             * Since we want the periodicity and and not aperiodicity:
             * periodicity = 1 - aperiodicity */
            probability = 1 - yinBuffer[tau];
            break;
        }
    }

    /* if no lastDetectedPitch found, tau => -1 */
    if (tau == halfBufferSize || yinBuffer[tau] >= threshold) {
        tau = -1;
        probability = 0;
    }

    return tau;
}

float YinPitchDetector::parabolicInterpolation(int16_t tauEstimate) {
    float betterTau;
    int x0;
    int x2;

    /* Calculate the first polynomial coeffcient based on the current estimate of tau */
    if (tauEstimate < 1) {
        x0 = tauEstimate;
    }
    else {
        x0 = tauEstimate - 1;
    }

    /* Calculate the second polynomial coeffcient based on the current estimate of tau */
    if (tauEstimate + 1 < halfBufferSize) {
        x2 = tauEstimate + 1;
    }
    else {
        x2 = tauEstimate;
    }

    /* Algorithm to parabolically interpolate the shift value tau to find a better estimate */
    if (x0 == tauEstimate) {
        if (yinBuffer[tauEstimate] <= yinBuffer[x2]) {
            betterTau = tauEstimate;
        }
        else {
            betterTau = x2;
        }
    }
    else if (x2 == tauEstimate) {
        if (yinBuffer[tauEstimate] <= yinBuffer[x0]) {
            betterTau = tauEstimate;
        }
        else {
            betterTau = x0;
        }
    }
    else {
        float s0, s1, s2;
        s0 = yinBuffer[x0];
        s1 = yinBuffer[tauEstimate];
        s2 = yinBuffer[x2];
        // fixed AUBIO implementation, thanks to Karl Helgason:
        // (2.0f * s1 - s2 - s0) was incorrectly multiplied with -1
        betterTau = tauEstimate + (s2 - s0) / (2 * (2 * s1 - s2 - s0));
    }


    return betterTau;
}

float YinPitchDetector::getFrequencyFromBuffer(const int16_t *buffer) {
    prepare();
    return getPitch(buffer);
}

void YinPitchDetector::init(int bufferSize, int sampleRate) {
    assert(bufferSize % 2 == 0);
    assert(sampleRate > 0);
    if (yinBuffer) {
        free(yinBuffer);
    }

    this->sampleRate = sampleRate;
    halfBufferSize = bufferSize / 2;
    yinBuffer = (float*) malloc(halfBufferSize * sizeof(float));
}

YinPitchDetector::~YinPitchDetector() {
    free(yinBuffer);
}

float YinPitchDetector::getThreshold() const {
    return threshold;
}

void YinPitchDetector::setThreshold(float threshold) {
    YinPitchDetector::threshold = threshold;
}

float YinPitchDetector::getProbability() const {
    return probability;
}

bool YinPitchDetector::hasThreshold() const {
    return true;
}
