//
// Created by Semyon Tikhonenko on 5/18/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PITCHINPUTREADERCOLLECTOR_H
#define VOCALTRAINER_PITCHINPUTREADERCOLLECTOR_H

#include <deque>
#include "Pitch.h"
#include "PitchInputReader.h"
#include "PitchesCollector.h"
#include "ListenersSet.h"
#include <functional>
#include <mutex>

class PitchInputReaderCollector : public PitchesCollector {
public:
    typedef CppUtils::ListenersSet<const Pitch&, double>::Listener PitchDetectedListener;
private:
    PitchInputReader* pitchInputReader = nullptr;

    std::deque<float> frequencies;
    std::deque<double> times;
    double savedPitchesTimeLimit = std::numeric_limits<double>::infinity();
    mutable std::mutex mutex;
public:
    CppUtils::ListenersSet<const Pitch&, double> pitchDetectedListeners;

    void init(AudioInputReader* audioInputReader,
            int smoothLevel,
            PitchDetector* pitchDetector);

    void operator()(const int16_t* data, int size);

    float getFrequencyAt(int index) const override;
    float getLastDetectedFrequency() const;
    Pitch getLastDetectedPitch() const;
    double getTimeAt(int index) const override;
    double getLastDetectedTime() const;
    int getPitchesCount() const override;

    virtual void setThreshold(float threshold);
    float getThreshold() const;
    ~PitchInputReaderCollector() override;
    virtual void pitchDetected(float frequency, double time);

    double getSavedPitchesTimeLimit() const;
    void setSavedPitchesTimeLimit(double savedPitchesTimeLimit);

    void clearCollectedPitches();

    int getPitchesCountAfterTime(double time) const override;
};


#endif //VOCALTRAINER_PITCHINPUTREADERCOLLECTOR_H
