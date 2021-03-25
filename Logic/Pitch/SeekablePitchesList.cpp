//
// Created by Semyon Tikhonenko on 2018-12-21.
//

#include "SeekablePitchesList.h"
#include "Algorithms.h"

#define LOCK std::lock_guard<std::mutex> _(mutex)

double SeekablePitchesList::getSeek() const {
    return seek;
}

void SeekablePitchesList::setSeek(double seek) {
    this->seek = seek;

    LOCK;
    if (!times.empty() && seek < times.back()) {
        // remove all pitches after seek
        auto iter = std::lower_bound(times.begin(), times.end(), seek);
        times.erase(iter, times.end());
        int index = static_cast<int>(times.end() - iter);
        CppUtils::EraseEndingOfCollection(frequencies, index);
    }
}

double SeekablePitchesList::appendPitch(float frequency) {
    double seek = this->seek;
    PitchesMutableList::appendPitch(seek, frequency);
    return seek;
}

SeekablePitchesList::SeekablePitchesList() {
    seek = 0;
}
