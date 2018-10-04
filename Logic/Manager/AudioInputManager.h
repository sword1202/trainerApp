//
// Created by Semyon Tikhonenko on 7/4/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H
#define VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H

#include "PitchInputReaderCollector.h"

class AudioInputManager : public PitchInputReaderCollector {
    AudioInputReaderWithOutput* audioInputReader = nullptr;
    int listenerKey = -1;
public:
    AudioInputManager(const char* deviceName);

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

    ~AudioInputManager();
};


#endif //VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H
