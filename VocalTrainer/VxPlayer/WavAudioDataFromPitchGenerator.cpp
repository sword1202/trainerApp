//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "WavAudioDataFromPitchGenerator.h"
#include "GetSf2FilePath.h"
#include "WAVFile.h"
#define TSF_IMPLEMENTATION
#include "tsf.h"

static const int SAMPLE_RATE = 44100;

WavAudioDataFromPitchGenerator::WavAudioDataFromPitchGenerator() {
    t = tsf_load_filename(GetSf2FilePath().data());
    tsf_set_output(t, TSF_MONO, SAMPLE_RATE, 0);
}

WavAudioDataFromPitchGenerator::~WavAudioDataFromPitchGenerator() {
    tsf_close(t);
}

std::vector<char> WavAudioDataFromPitchGenerator::pitchToWavAudioData(const Pitch &pitch, double duration) {
    tsf_note_on(t, 0, pitch.getSoundFont2Index(), 1.0f);
    std::vector<short> pcmData(SAMPLE_RATE * duration);
    tsf_render_short(t, pcmData.data(), pcmData.size(), 0);
    return WAVFile::addWavHeaderToRawPcmData((char*)pcmData.data(), pcmData.size() * 2);
}
