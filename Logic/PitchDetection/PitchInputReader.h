//
// Created by Semyon Tikhonenko on 8/7/16.
//

#ifndef PITCHDETECTION_PITCHINPUTREADER_H
#define PITCHDETECTION_PITCHINPUTREADER_H


#include "AudioInputReader.h"
#include "Pitch.h"
#include "PitchDetector.h"
#include "PitchDetectionSmoothingAudioBuffer.h"
#include <memory>

class PitchInputReader {
    std::unique_ptr<PitchDetector> pitchDetector;
    std::function<void(Pitch)> callback;
    Pitch lastDetectedPitch;
    PitchDetectionSmoothingAudioBuffer smoothingAudioBuffer;
    bool executeCallBackOnInvalidPitches = false;
public:
    PitchInputReader(AudioInputReader* audioInputReader, PitchDetector* pitchDetector, int smoothLevel);
    void operator()(const int16_t* data, int size);

    void setCallback(const std::function<void(Pitch)>& callback);
    const Pitch &getLastDetectedPitch() const;

    PitchDetector* getPitchDetector() const;

    bool willExecuteCallBackOnInvalidPitches() const;
    void setExecuteCallBackOnInvalidPitches(bool executeCallBackOnInvalidPitches);

    ~PitchInputReader();
};

#endif //PITCHDETECTION_PITCHINPUTREADER_H
