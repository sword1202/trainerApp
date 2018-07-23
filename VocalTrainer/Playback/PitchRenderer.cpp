//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "PitchRenderer.h"

PitchRenderer::~PitchRenderer() {

}

int PitchRenderer::getSampleRate() const {
    return sampleRate;
}

PitchRenderer::PitchRenderer(int sampleRate) : sampleRate(sampleRate) {
}
