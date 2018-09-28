//
// Created by Semyon Tikhonenko on 7/4/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H
#define VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H

#include "PitchInputReaderCollector.h"
#include "RealtimeStreamingAudioPlayer.h"

class PitchInputReaderAndPlayer : public PitchInputReaderCollector {
    AudioInputReader* audioInputReader;
    RealtimeStreamingAudioPlayer* audioInputPlayer;
public:
    PitchInputReaderAndPlayer();

    void setInputVolume(float value);
    float getInputVolume() const;
    void setOutputVolume(float value);
    float getOutputVolume() const;

    void start() override;
    void stop() override;

    ~PitchInputReaderAndPlayer();
    void pitchDetected(float frequency, double time) override;
};


#endif //VOCALTRAINER_VOCALTRAINERPITCHINPUTREADER_H
