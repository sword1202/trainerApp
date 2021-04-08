//
// Created by Semyon Tikhonenko on 2018-12-21.
//

#include "PitchesMutableList.h"
#include "Algorithms.h"

#define LOCK std::lock_guard<std::mutex> _(mutex)

void PitchesMutableList::getPitchesInTimeRange(
        double begin,
        double end,
        std::vector<double> *timesOut,
        std::vector<float> *frequenciesOut
        ) const {
    LOCK;
    auto range = CppUtils::FindRangeInSortedCollection(times, begin, end);
    size_t i1 = range.first - times.begin();
    size_t i2 = range.second - times.begin();

    size_t size = i2 - i1;
    frequenciesOut->resize(size);
    timesOut->resize(size);

    std::copy(range.first, range.second, timesOut->begin());
    std::copy(frequencies.begin() + i1, frequencies.begin() + i2, frequenciesOut->begin());
}

void PitchesMutableList::appendPitch(double time, float frequency) {
    LOCK;
    frequencies.push_back(frequency);
    times.push_back(time);
}

std::vector<double> PitchesMutableList::getTimes() const {
    LOCK;
    return times;
}

std::vector<float> PitchesMutableList::getFrequencies() const {
    LOCK;
    return frequencies;
}

Pitch PitchesMutableList::getNearestPitch(double time) const {
    LOCK;
    auto iter = CppUtils::FindNearestElementInSortedCollection(times.begin(), times.end(), time);
    if (iter == times.end()) {
        return Pitch();
    }

    size_t index = iter - times.begin();
    return Pitch(frequencies[index]);
}

PitchesMutableList::PitchesMutableList(const std::vector<float> &frequencies, const std::vector<double> &times)
        : frequencies(frequencies), times(times) {}

PitchesMutableList::PitchesMutableList(std::vector<float> &&frequencies, std::vector<double> &&times) {
    this->frequencies = std::move(frequencies);
    this->times = std::move(times);
}

void PitchesMutableList::clearPitches() {
    LOCK;
    frequencies.clear();
    times.clear();
}
