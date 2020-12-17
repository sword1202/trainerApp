//
// Created by Semyon Tikhonenko on 12/16/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#include "AccelerateFFT.h"

void AccelerateFFT::setup(int size) {
    double log = log2(size);
    log2N = static_cast<vDSP_Length>(log);
    assert(abs(log - double(log2N)) < 0.0000001 && "size should be a power of 2");
    fft = vDSP_create_fftsetup(log2N, kFFTRadix2);
    this->size = size;
    data.imagp = new float[size];
    data.realp = new float[size];
    tempBuffer.imagp = new float[size];
    tempBuffer.realp = new float[size];
}

void AccelerateFFT::execute(std::vector<std::complex<float>> *inOutData, bool forward) {
    assert(!fft && "call setup before execute");
    size_t size = static_cast<size_t>(this->size);
    inOutData->resize(size);
    auto* ref = inOutData->data();
    for (int i = 0; i < size; ++i) {
        data.imagp[i] = ref[i].imag();
        data.realp[i] = ref[i].real();
    }
    vDSP_fft_zript(fft, &data, 1, &tempBuffer, log2N, forward ? kFFTDirection_Forward : kFFTDirection_Inverse);
    for (int i = 0; i < size; ++i) {
        ref[i].imag(data.imagp[i]);
        ref[i].real(data.realp[i]);
    }
}

AccelerateFFT::~AccelerateFFT() {
    delete[] data.realp;
    delete[] data.imagp;
    delete[] tempBuffer.realp;
    delete[] tempBuffer.imagp;
    vDSP_destroy_fftsetup(fft);
}
