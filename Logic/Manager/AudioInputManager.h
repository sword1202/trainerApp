//
// Created by Semyon Tikhonenko on 7/4/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H
#define VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H

#include "AudioInputPitchesRecorder.h"
#include "AudioInputRecorder.h"

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

    void startPitchDetection();
    void stopPitchDetection();

    const char * getInputDeviceName() const;
    void setInputDeviceName(const char *deviceName) const;

    void addAudioInputReaderCallback(const AudioInputReader::Callback& callback);
    void addAudioInputLevelMonitor(const std::function<void(double)>& callback);

    bool isAudioRecordingEnabled() const;
    void setAudioRecordingEnabled(bool audioDataCollectorEnabled);
    void setAudioRecorderSeek(double timeSeek);
    void setPitchesRecorderSeek(double timeSeek);

    CppUtils::ListenersSet<const Pitch&, double >& getPitchDetectedListeners();

    AudioInputPitchesRecorder *getPitchesRecorder() const;

    ~AudioInputManager();
};


#endif //VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H
