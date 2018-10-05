//
// Created by Semyon Tikhonenko on 9/30/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "AudioAverageInputLevelMonitor.h"
#include "Algorithms.h"
#include "AudioUtils.h"

constexpr int BUFFER_SIZE = 1024;
constexpr double THRESHOLD = 60;

void AudioAverageInputLevelMonitor::operator()(const int16_t *data, int size) const {
    AudioUtils::Int16SamplesIntoFloatSamples(data, size, tempFloatBuffer.data());
    double sum = CppUtils::AbsoluteAverage<double>(tempFloatBuffer.data(), (int)tempFloatBuffer.size());
    double value = 20 * log10(sum) + THRESHOLD;
    double inputLevel = value < 0 ? 0 : value / THRESHOLD;
    callback(inputLevel);
}

AudioAverageInputLevelMonitor::AudioAverageInputLevelMonitor(int bufferSize, const Callback& callback)
: callback(callback) {
    tempFloatBuffer.resize((size_t)bufferSize);
}
