//
// Created by Semyon Tikhonenko on 12/16/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_FFT_H
#define VOCALTRAINER_FFT_H

#include <complex>
#include <vector>

class FFT {
public:
    virtual void setup(int size) = 0;
    virtual void execute(std::vector<std::complex<float>> *inOutData, bool forward) = 0;
    static FFT* create();
};


#endif //VOCALTRAINER_FFT_H
