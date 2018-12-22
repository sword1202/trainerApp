//
// Created by Semyon Tikhonenko on 9/30/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <cmath>

#include "AudioAverageInputLevelMonitor.h"
#include "Algorithms.h"
#include "AudioUtils.h"

constexpr double THRESHOLD = 60;

void AudioAverageInputLevelMonitor::operator()(const int16_t *data, int size) const {
    tempFloatBuffer.resize(size);
    AudioUtils::Int16SamplesIntoFloatSamples(data, size, tempFloatBuffer.data());
    double sum = CppUtils::AbsoluteAverage<double>(tempFloatBuffer.data(), (int)tempFloatBuffer.size());
    double value = 20 * log10(sum) + THRESHOLD;
    double inputLevel = value < 0 ? 0 : value / THRESHOLD;
    callback(inputLevel);
}

AudioAverageInputLevelMonitor::AudioAverageInputLevelMonitor(const Callback& callback)
: callback(callback) {
    tempFloatBuffer.reserve(2048);
}
