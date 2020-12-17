//
// Created by Semyon Tikhonenko on 2018-12-21.
//

#ifndef VOCALTRAINER_SEEKABLEPITCHESLIST_H
#define VOCALTRAINER_SEEKABLEPITCHESLIST_H

#include "PitchesMutableList.h"

class SeekablePitchesList : public PitchesMutableList {
    std::atomic<double> seek;
public:
    SeekablePitchesList();

    double getSeek() const;
    void setSeek(double seek);
    double appendPitch(float frequency);
};


#endif //VOCALTRAINER_SEEKABLEPITCHESLIST_H
