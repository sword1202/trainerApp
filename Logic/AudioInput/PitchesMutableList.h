//
// Created by Semyon Tikhonenko on 2018-12-21.
//

#ifndef VOCALTRAINER_PITCHESLISTCOLLECTION_H
#define VOCALTRAINER_PITCHESLISTCOLLECTION_H

#include "PitchesCollection.h"
#include <mutex>
#include <vector>

class PitchesMutableList : public PitchesCollection {
protected:
    std::vector<float> frequencies;
    std::vector<double> times;
    mutable std::mutex mutex;
public:
    void getPitchesInTimeRange(double begin, double end, std::vector<double> *timesOut,
                               std::vector<float> *frequenciesOut) const override;

    void appendPitch(double time, float frequency);
    std::vector<double> getTimes() const;
    std::vector<float> getFrequencies() const;

    Pitch getNearestPitch(double time) const override;
};


#endif //VOCALTRAINER_PITCHESLISTCOLLECTION_H
