//
// Created by Semyon Tikhonenko on 7/4/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H
#define VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H

#include "AudioInputPitchesRecorder.h"
#include "AudioInputRecorder.h"
#include "DestructorQueue.h"

class AudioInputManager {
    AudioInputReaderWithOutput* audioInputReader = nullptr;
    AudioInputRecorder* audioRecorder = nullptr;
    AudioInputPitchesRecorder* pitchesRecorder;
    bool audioRecordingEnabled = true;
public:
    explicit AudioInputManager(const char* deviceName);

    void setInputVolume(float value);
    float getInputVolume() const;
    void setOutputVolume(float value);
    float getOutputVolume() const;

    void startPitchDetection(double seek);
    void stopPitchDetection();

    const char * getInputDeviceName() const;
    void setInputDeviceName(const char *deviceName) const;

    void addAudioInputReaderCallback(const AudioInputReader::Callback& callback, CppUtils::AbstractDestructorQueue* parent);
    void addAudioInputLevelMonitor(const std::function<void(double)>& callback, CppUtils::AbstractDestructorQueue* parent);

    bool isAudioRecordingEnabled() const;
    void setAudioRecordingEnabled(bool audioDataCollectorEnabled);
    std::string getRecordedDataInWavFormat() const;
    void setAudioRecorderSeek(double timeSeek);
    void setPitchesRecorderSeek(double timeSeek);

    CppUtils::ListenersSet<const Pitch&, double >& getPitchDetectedListeners();
    const PitchesCollection *getRecordedPitches() const;

    ~AudioInputManager();
};


#endif //VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H
