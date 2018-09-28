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
    double savedPitchesTimeLimit = 10.0;
    mutable std::mutex mutex;

    CppUtils::ListenersSet<const Pitch&, double> pitchDetectedListeners;
protected:
    virtual void setAudioInputReaderCallback(AudioInputReader* audioInputReader,
            const AudioInputReader::Callback& callback);
public:
    void init(AudioInputReader* audioInputReader,
            int smoothLevel,
            PitchDetector* pitchDetector,
            bool deleteAudioInputReaderOnDestructor = true);
    float getFrequencyAt(int index) const override;
    float getLastDetectedFrequency() const;
    Pitch getLastDetectedPitch() const;
    double getTimeAt(int index) const override;
    double getLastDetectedTime() const;
    int getPitchesCount() const override;
    bool isRunning() const;

    virtual void setThreshold(float threshold);
    float getThreshold() const;
    virtual ~PitchInputReaderCollector();
    virtual void pitchDetected(float frequency, double time);

    double getSavedPitchesTimeLimit() const;
    void setSavedPitchesTimeLimit(double savedPitchesTimeLimit);

    virtual void start();
    virtual void stop();

    int addPitchDetectedListener(const PitchDetectedListener& listener);
    void removePitchDetectedListener(int key);
};


#endif //VOCALTRAINER_PITCHINPUTREADERCOLLECTOR_H
