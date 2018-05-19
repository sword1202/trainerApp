//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "SoundFont2PitchRenderer.h"
#define TSF_IMPLEMENTATION
#include "tsf.h"
#include "AudioUtils.h"
#include "LoadTsf.h"

constexpr float VOLUME = 0.5;

void SoundFont2PitchRenderer::render(const Pitch &pitch, short *buffer, int length) {
    tsf_note_on(_tsf, 0, pitch.getSoundFont2Index(), VOLUME);
    tsf_render_short(_tsf, buffer, length, 0);
    AudioUtils::MakeLinearFadeInAtBeginning(buffer, length, smoothingSize);
    AudioUtils::MakeLinearFadeOutAtEnding(buffer, length, smoothingSize);
}

SoundFont2PitchRenderer::SoundFont2PitchRenderer(int sampleRate, double smoothingRangeInSeconds) :
        PitchRenderer(sampleRate) {
    smoothingSize = (int)round(sampleRate * smoothingRangeInSeconds);
    _tsf = LoadTsf();
    tsf_set_output(_tsf, TSF_MONO, sampleRate, 0);
}

SoundFont2PitchRenderer::~SoundFont2PitchRenderer() {
    tsf_close(_tsf);
}
