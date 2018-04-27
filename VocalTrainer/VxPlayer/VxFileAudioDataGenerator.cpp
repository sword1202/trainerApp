//
// Created by Semyon Tikhonenko on 4/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <cmath>
#include <boost/assert.hpp>
#include "VxFileAudioDataGenerator.h"
#include "GetSf2FilePath.h"
#include "AudioUtils.h"
#define TSF_IMPLEMENTATION
#include "tsf.h"

constexpr double PITCH_AUDIO_FADE_PERCENTAGE = 0.2;

VxFileAudioDataGenerator::VxFileAudioDataGenerator(const VxFile *vxFile,
        const VxFileAudioDataGeneratorConfig &config) {
    sampleRate = config.sampleRate;
    outBufferSize = config.outBufferSize;
    this->vxFile = vxFile;

    double durationInSeconds = vxFile->getDurationInSeconds();
    int pcmDataSize = (int)ceil(durationInSeconds * sampleRate);
    pcmData.assign(pcmDataSize, 0);
    initializedPcmDataFlags.resize(pcmDataSize, false);
    bufferLengthInSeconds = AudioUtils::GetSampleTimeInSeconds(outBufferSize, sampleRate);

    _tsf = tsf_load_filename(GetSf2FilePath().data());
    tsf_set_output(_tsf, TSF_MONO, sampleRate, 0);
}

VxFileAudioDataGenerator::VxFileAudioDataGenerator(const VxFile *vxFile) :
        VxFileAudioDataGenerator(vxFile, VxFileAudioDataGeneratorConfig()) {
    
}

void VxFileAudioDataGenerator::reset() {
    seek = 0;
    std::fill(pcmData.begin(), pcmData.end(), 0);
    std::fill(initializedPcmDataFlags.begin(), initializedPcmDataFlags.end(), false);
}

void VxFileAudioDataGenerator::renderNextPitch() {
    const std::vector<VxPitch> &pitches = vxFile->getPitches();
    BOOST_ASSERT(renderedPitchesCount < pitches.size());
    const VxPitch &pitch = pitches[renderedPitchesCount];

    int begin = vxFile->samplesCountFromTicks(pitch.startTickNumber, sampleRate);
    int length = vxFile->samplesCountFromTicks(pitch.ticksCount, sampleRate);

    std::vector<short> temp(length);
    tsf_note_on(_tsf, 0, pitch.pitch.getSoundFont2Index(), 0.5);
    tsf_render_short(_tsf, temp.data(), length, 0);
    AudioUtils::MakeLinearFadeInAtBeginning(temp.data(), length, PITCH_AUDIO_FADE_PERCENTAGE);
    AudioUtils::MakeLinearFadeOutAtEnding(temp.data(), length, PITCH_AUDIO_FADE_PERCENTAGE);

    for (int i = 0; i < length; ++i) {
        short value = temp[i];
        if (initializedPcmDataFlags[i + begin]) {
            int pcmValue = pcmData[i + begin];
            pcmData[i + begin] = (short)((pcmValue + value) / 2);
        } else {
            pcmData[i + begin] = value;
            initializedPcmDataFlags[i + begin] = true;
        }
    }
    
    renderedDataSize = begin;
    if (renderedPitchesCount >= pitches.size() - 1) {
        renderedDataSize = pcmData.size();
    } else {
        const VxPitch &nextPitch = pitches[renderedPitchesCount + 1];
        if (pitch.endTickNumber() > nextPitch.startTickNumber) {
            renderedDataSize += vxFile->samplesCountFromTicks(nextPitch.startTickNumber, sampleRate);
        } else {
            renderedDataSize += length;
        }
    }

    renderedPitchesCount++;
}

VxFileAudioDataGenerator::~VxFileAudioDataGenerator() {
    tsf_close(_tsf);
}

bool VxFileAudioDataGenerator::canRenderNextPitch() const {
    return renderedPitchesCount < vxFile->getPitches().size();
}

int VxFileAudioDataGenerator::readNextSamplesBatch(short *intoBuffer) {
    while (seek + outBufferSize < renderedDataSize && canRenderNextPitch()) {
        renderNextPitch();
    }

    int size = std::min(renderedDataSize - seek, outBufferSize);
    std::copy(pcmData.begin() + seek, pcmData.begin() + seek + size, intoBuffer);
    seek += size;
    return size;
}
