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
    AudioInputReader* audioInputReader;
    std::function<void(Pitch)> callback;
    bool destroyAudioInputReaderOnDestructor = true;
    Pitch lastDetectedPitch;
    PitchDetectionSmoothingAudioBuffer smoothingAudioBuffer;
    bool executeCallBackOnInvalidPitches = false;
    //bool running;
public:
    PitchInputReader(AudioInputReader *audioInputReader, PitchDetector* pitchDetector, int smoothLevel);
    bool willDestroyAudioInputReaderOnDestructor() const;
    void setDestroyAudioInputReaderOnDestructor(bool destroyAudioInputReaderOnDestructor);
    void setCallback(const std::function<void(Pitch)>& callback);
    void start();
    void stop();
    const Pitch &getLastDetectedPitch() const;

    PitchDetector* getPitchDetector() const;

    bool willExecuteCallBackOnInvalidPitches() const;
    void setExecuteCallBackOnInvalidPitches(bool executeCallBackOnInvalidPitches);

    bool isRunning() const;

    ~PitchInputReader();
};

#endif //PITCHDETECTION_PITCHINPUTREADER_H
