//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "SoundFont2PitchRenderer.h"
#define TSF_IMPLEMENTATION
#include "tsf.h"
#include "AudioUtils.h"
#include "GetSf2FilePath.h"

constexpr float VOLUME = 0.5;
constexpr double PITCH_AUDIO_FADE_PERCENTAGE = 0.2;

void SoundFont2PitchRenderer::render(const Pitch &pitch, short *buffer, int length) {
    tsf_note_on(_tsf, 0, pitch.getSoundFont2Index(), VOLUME);
    tsf_render_short(_tsf, buffer, length, 0);
    AudioUtils::MakeLinearFadeInAtBeginning(buffer, length, PITCH_AUDIO_FADE_PERCENTAGE);
    AudioUtils::MakeLinearFadeOutAtEnding(buffer, length, PITCH_AUDIO_FADE_PERCENTAGE);
}

SoundFont2PitchRenderer::SoundFont2PitchRenderer(int sampleRate) : PitchRenderer(sampleRate) {
    _tsf = tsf_load_filename(GetSf2FilePath().data());
    tsf_set_output(_tsf, TSF_MONO, sampleRate, 0);
}
