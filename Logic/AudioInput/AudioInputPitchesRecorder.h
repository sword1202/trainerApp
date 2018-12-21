//
// Created by Semyon Tikhonenko on 5/18/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PITCHINPUTREADERCOLLECTOR_H
#define VOCALTRAINER_PITCHINPUTREADERCOLLECTOR_H

#include <vector>
#include "Pitch.h"
#include "PitchInputReader.h"
#include "PitchesRecorder.h"
#include "ListenersSet.h"
#include <functional>
#include <mutex>

class AudioInputPitchesRecorder : public PitchesRecorder {
public:
    typedef CppUtils::ListenersSet<const Pitch&, double>::Listener PitchDetectedListener;
private:
    PitchInputReader* pitchInputReader = nullptr;

    std::vector<float> frequencies;
    std::vector<double> times;
    mutable std::mutex mutex;
    std::atomic<double> seek;
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
    ~AudioInputPitchesRecorder() override;
    virtual void pitchDetected(float frequency, double time);

    double getSeek() const;
    void setSeek(double seek);

    int getPitchesCountAfterTime(double time) const override;
};


#endif //VOCALTRAINER_PITCHINPUTREADERCOLLECTOR_H
