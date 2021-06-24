//
// Created by Semyon Tikhonenko on 5/18/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PITCHINPUTREADERCOLLECTOR_H
#define VOCALTRAINER_PITCHINPUTREADERCOLLECTOR_H

#include <vector>
#include "Pitch.h"
#include "PitchInputReader.h"
#include "PitchesCollection.h"
#include "SeekablePitchesList.h"
#include "ListenersSet.h"
#include "Executors.h"
#include <functional>
#include <mutex>

class AudioInputPitchesRecorder : private CppUtils::OnThreadExecutor {
    PitchInputReader* pitchInputReader = nullptr;
    SeekablePitchesList pitches;
public:
    CppUtils::ListenersSet<const Pitch&, double> pitchDetectedListeners;

    void init(AudioInputReader* audioInputReader,
            int smoothLevel,
            PitchDetector* pitchDetector);

    void operator()(const int16_t* data, int size);

    ~AudioInputPitchesRecorder();
    virtual void pitchDetected(float frequency, double time);

    void setSeek(double seek);

    const PitchesCollection* getPitches() const;

    void clearRecordedPitches();
};


#endif //VOCALTRAINER_PITCHINPUTREADERCOLLECTOR_H
