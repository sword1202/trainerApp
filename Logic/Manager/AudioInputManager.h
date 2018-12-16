//
// Created by Semyon Tikhonenko on 7/4/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H
#define VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H

#include "PitchInputReaderCollector.h"
#include "AudioInputDataCollector.h"

class AudioInputManager : public PitchInputReaderCollector {
    AudioInputReaderWithOutput* audioInputReader = nullptr;
    AudioInputDataCollector* dataCollector = nullptr;
    bool audioDataCollectorEnabled = false;
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

    bool isAudioDataCollectorEnabled() const;
    void setAudioDataCollectorEnabled(bool audioDataCollectorEnabled);
    void setDataCollectorSeek(double time);

    ~AudioInputManager() override;
};


#endif //VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H
