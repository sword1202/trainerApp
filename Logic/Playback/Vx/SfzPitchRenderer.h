//
// Created by Semyon Tikhonenko on 12/13/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_SFZPITCHRENDERER_H
#define VOCALTRAINER_SFZPITCHRENDERER_H

#include "PitchRenderer.h"
#include "liquidsfz.hh"

class SfzPitchRenderer : public PitchRenderer {
    static LiquidSFZ::Synth* sfz;
    static bool isSfzLocked;
    constexpr static int NUMBER_OF_OUTPUTS = 2;
    int numberOfChannels = -1;
    float* outputs[NUMBER_OF_OUTPUTS];
    int maxFramesPerBuffer;
public:
    static void initSfz(const std::string& sfzFilePath);

    ~SfzPitchRenderer();
    void init(int sampleRate, int numberOfChannels, int maxFramesPerBuffer) override;
    void on(const Pitch &pitch) override;
    void off(const Pitch &pitch) override;
    void render(int16_t *outBuffer, int framesCount) override;
};


#endif //VOCALTRAINER_SFZPITCHRENDERER_H
