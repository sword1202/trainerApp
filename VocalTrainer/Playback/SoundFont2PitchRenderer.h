//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_SOUNDFONT2PITCHRENDERER_H
#define VOCALTRAINER_SOUNDFONT2PITCHRENDERER_H

#include "PitchRenderer.h"
#include "tsf.h"

class SoundFont2PitchRenderer : public PitchRenderer {
    tsf* _tsf;
    int smoothingSize;
public:
    SoundFont2PitchRenderer(int sampleRate, double smoothingRangeInSeconds = 0);
    ~SoundFont2PitchRenderer();
    void render(const Pitch &pitch, short *buffer, int length) override;
};


#endif //VOCALTRAINER_SOUNDFONT2PITCHRENDERER_H
