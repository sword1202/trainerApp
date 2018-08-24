//
// Created by Semyon Tikhonenko on 7/2/18.
//

#ifndef PITCHDETECTION_AUBIOPITCHDETECTOR_H
#define PITCHDETECTION_AUBIOPITCHDETECTOR_H

#include "PitchDetector.h"
#include "aubio/aubio.h"
#include "CppUtils/Functions.h"

class AubioPitchDetector : public PitchDetector {
public:
    enum Method {
        YIN, // This algorithm was developed by A. de Cheveigne and H. Kawahara and was first published in:
        // De Cheveigné, A., Kawahara, H. (2002) "YIN, a fundamental frequency estimator for speech and music",
        // J. Acoust. Soc. Am. 111, 1917-1930.

        YINFFT, // This algorithm was derived from the YIN algorithm.
        // In this implementation, a Fourier transform is used to compute a tapered square difference function,
        // which allows spectral weighting.
        // Because the difference function is tapered, the selection of the period is simplified.

        SCHMITT, // This pitch extraction method implements a Schmitt trigger to estimate the period of a signal.
        // It is computationally very inexpensive, but also very sensitive to noise.

        FCOMB, // This pitch extraction method implements a fast harmonic comb filter to determine
        // the fundamental frequency of a harmonic sound.

        MCOMB, //This fundamental frequency estimation algorithm implements spectral flattening,
        // multi-comb filtering and peak histogramming.

        SPECACF // Spectral auto-correlation function
    };

private:
    Method method;
    aubio_pitch_t* aubio = nullptr;
    fvec_t* in;

    CppUtils::Functions::InitQueue onInitQueue;
public:

    void init(int maxBufferSize, int sampleRate) override;
    float getFrequencyFromBuffer(const int16_t *buffer) override;
    float getThreshold() const override;
    void setThreshold(float threshold) override;
    bool hasThreshold() const override;

    explicit AubioPitchDetector(Method method);
    AubioPitchDetector();
    ~AubioPitchDetector();
};


#endif //PITCHDETECTION_AUBIOPITCHDETECTOR_H
