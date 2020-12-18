//
// Created by Semyon Tikhonenko on 12/16/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_ACCELERATEFFT_H
#define VOCALTRAINER_ACCELERATEFFT_H

#include "FFT.h"
#include <Accelerate/Accelerate.h>

class AccelerateFFT : public FFT {
    FFTSetup fft = nullptr;
    DSPSplitComplex data;
    DSPSplitComplex tempBuffer;
    int size;
    vDSP_Length log2N;
public:
    void setup(int size) override;
    void execute(std::vector<std::complex<float>> *inOutData, bool forward) override;
    ~AccelerateFFT();
};


#endif //VOCALTRAINER_ACCELERATEFFT_H
