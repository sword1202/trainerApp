//
// Created by Semyon Tikhonenko on 8/3/18.
//

#ifndef QTVOCALTRAINER_BOUNDS_H
#define QTVOCALTRAINER_BOUNDS_H

#include <iostream>

class PlaybackBounds {
    double startSeek;
    double endSeek;
public:
    PlaybackBounds();
    PlaybackBounds(double startSeek, double endSeek);
    double getStartSeek() const;
    double getEndSeek() const;

    void setStartSeek(double startSeek);
    void setEndSeek(double endSeek);

    double getDuration() const;
    bool isInside(double value) const;
    operator bool() const;

    bool operator==(const PlaybackBounds &rhs) const;
    bool operator!=(const PlaybackBounds &rhs) const;
};

std::ostream& operator<<(std::ostream& os, const PlaybackBounds& bounds);

#endif //QTVOCALTRAINER_BOUNDS_H
