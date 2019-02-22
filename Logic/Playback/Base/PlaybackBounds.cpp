#include <assert.h>
#include "PlaybackBounds.h"

PlaybackBounds::PlaybackBounds(double startSeek, double endSeek) : startSeek(startSeek), endSeek(endSeek) {
    assert(startSeek < endSeek && startSeek >= 0 && endSeek > 0);
}

double PlaybackBounds::getStartSeek() const {
    assert(startSeek >= 0 && startSeek < endSeek);
    return startSeek;
}

double PlaybackBounds::getEndSeek() const {
    assert(startSeek >= 0 && startSeek < endSeek);
    return endSeek;
}

bool PlaybackBounds::isInside(double value) const {
    return value >= startSeek && value <= endSeek;
}

bool PlaybackBounds::operator==(const PlaybackBounds &rhs) const {
    return startSeek == rhs.startSeek &&
            endSeek == rhs.endSeek;
}

bool PlaybackBounds::operator!=(const PlaybackBounds &rhs) const {
    return !(rhs == *this);
}

PlaybackBounds::operator bool() const {
    return startSeek >= 0;
}

PlaybackBounds::PlaybackBounds() {
    startSeek = -1;
    endSeek = -1;
}

double PlaybackBounds::getDuration() const {
    assert(startSeek >= 0 && startSeek < endSeek);
    return endSeek - startSeek;
}

void PlaybackBounds::setStartSeek(double startSeek) {
    this->startSeek = startSeek;
}

void PlaybackBounds::setEndSeek(double endSeek) {
    this->endSeek = endSeek;
}

std::ostream& operator<<(std::ostream& os, const PlaybackBounds& bounds) {
    os<<"["<<bounds.getStartSeek()<<","<<bounds.getEndSeek()<<"]\n";
    return os;
}
