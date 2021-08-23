//
// Created by Semyon Tikhonenko on 12/13/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#include "SfzPitchRenderer.h"
#include "MathUtils.h"
#include <iostream>

using std::cout;
using std::endl;
using namespace CppUtils;

#define CHECK_NUMBER_OF_CHANNELS assert(numberOfChannels >= 1 && "wrong number of channels, don't forget to call init before")

void SfzPitchRenderer::init(int sampleRate, int numberOfChannels, int maxFramesPerBuffer) {
    assert(sfzLoader && "Call initSfz before using SfzPitchRenderer");
    assert(this->numberOfChannels == -1 && "init should not be called twice");
    assert(maxFramesPerBuffer > 0);
    assert(numberOfChannels > 0 && numberOfChannels <= NUMBER_OF_OUTPUTS);
    this->maxFramesPerBuffer = maxFramesPerBuffer;
    this->numberOfChannels = numberOfChannels;
    sfz = new LiquidSFZ::Synth();
    sfz->load(sfzLoader);
    sfz->system_reset();
    sfz->set_sample_rate(static_cast<uint>(sampleRate));
    for (int i = 0; i < NUMBER_OF_OUTPUTS; ++i) {
        outputs[i] = new float[maxFramesPerBuffer];
    }
}

void SfzPitchRenderer::on(const Pitch &pitch) {
    CHECK_NUMBER_OF_CHANNELS;
    sfz->add_event_note_on(0, 0, pitch.getMidiIndex(), 100);
}

void SfzPitchRenderer::off(const Pitch &pitch) {
    CHECK_NUMBER_OF_CHANNELS;
    sfz->add_event_note_off(0, 0, pitch.getMidiIndex());
}

void SfzPitchRenderer::render(int16_t *outBuffer, int framesCount) {
    assert(framesCount > 0);
    assert(framesCount <= maxFramesPerBuffer);
    CHECK_NUMBER_OF_CHANNELS;
    sfz->process(outputs, static_cast<uint>(framesCount));
    int maxSample = std::numeric_limits<short>::max();
    switch (this->numberOfChannels) {
        case NUMBER_OF_OUTPUTS:
            for (int i = 0; i < framesCount; ++i) {
                for (int channel = 0; channel < NUMBER_OF_OUTPUTS; ++channel) {
                    short shortSample = Math::RoundToShort(outputs[channel][i] * maxSample);
                    outBuffer[i * NUMBER_OF_OUTPUTS + channel] = shortSample;
                }
            }
            break;
        case 1:
            for (int i = 0; i < framesCount; ++i) {
                short shortSample = Math::RoundToShort((outputs[0][i] + outputs[1][i]) / 2.0f * maxSample);
                outBuffer[i] = shortSample;
            }
            break;
        default:
            assert(false);
    }
}

LiquidSFZInternal::Loader* SfzPitchRenderer::sfzLoader = nullptr;

void SfzPitchRenderer::initSfz(const std::string &sfzFilePath) {
    LiquidSFZ::Synth temp;
    assert(!sfzLoader && "sfz has been already initialized");
    sfzLoader = temp.createLoader(sfzFilePath);
}

SfzPitchRenderer::~SfzPitchRenderer() {
    delete sfz;
    for (int i = 0; i < NUMBER_OF_OUTPUTS; ++i) {
        delete[] outputs[i];
    }
}
