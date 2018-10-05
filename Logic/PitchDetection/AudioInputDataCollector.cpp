//
// Created by Semyon Tikhonenko on 10/5/18.
//

#include "AudioInputDataCollector.h"

void AudioInputDataCollector::operator()(const int16_t *data, int size) {
    collectedData.insert(collectedData.end(), data, data + size);
}

const std::deque<int16_t> &AudioInputDataCollector::getCollectedData() const {
    return collectedData;
}

void AudioInputDataCollector::clearCollectedData() {
    collectedData.clear();
}
