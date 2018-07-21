//
// Created by Semyon Tikhonenko on 7/20/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <cmath>
#include "VxFileAudioDataGenerator.h"
#include "AudioUtils.h"
#include "SoundFont2PitchRenderer.h"
#include <iostream>
#include "MemoryUtils.h"
#include "LoadTsf.h"
#include "Sets.h"
#include "StringUtils.h"
#include <iostream>

using namespace CppUtils;
using namespace AudioUtils;

using std::cout;
using std::endl;

int VxFileAudioDataGenerator::readNextSamplesBatch(short *intoBuffer) {
    int size = std::min(pcmDataSize - seek, outBufferSize);

    double startTime = GetSampleTimeInSeconds(seek, sampleRate);
    double endTime = GetSampleTimeInSeconds(seek + size, sampleRate);

    tempPitchIndexes.clear();
    vxFile.getPitchesIndexesInTimeRange(startTime, endTime, std::back_inserter(tempPitchIndexes));

    difference.clear();
    Sets::Difference(pitchesIndexes, tempPitchIndexes, std::back_inserter(difference));
    for (int pitchIndex : difference) {
        const Pitch& pitch = vxFile.getPitches()[pitchIndex].pitch;
        tsf_note_off(_tsf, 0, pitch.getSoundFont2Index());
    }

    difference.clear();
    Sets::Difference(tempPitchIndexes, pitchesIndexes, std::back_inserter(difference));
    for (int pitchIndex : difference) {
        const Pitch& pitch = vxFile.getPitches()[pitchIndex].pitch;
        tsf_note_on(_tsf, 0, pitch.getSoundFont2Index(), 1.0f);
    }

    tsf_render_short(_tsf, intoBuffer, size, 0);

    tempPitchIndexes.swap(pitchesIndexes);

    seek += size;

    return size;
}

VxFileAudioDataGenerator::VxFileAudioDataGenerator(const VxFile& vxFile, const VxFileAudioDataGeneratorConfig &config)
        : vxFile(vxFile), outBufferSize(config.outBufferSize), sampleRate(config.sampleRate) {
    double durationInSeconds = vxFile.getDurationInSeconds();
    pcmDataSize = (int)ceil(durationInSeconds * sampleRate);
    pitchesIndexes.reserve(10);
    tempPitchIndexes.reserve(10);
    difference.reserve(10);

    _tsf = LoadTsf();
    tsf_set_output(_tsf, TSF_MONO, sampleRate, 0);
}

VxFileAudioDataGenerator::VxFileAudioDataGenerator(const VxFile &vxFile) : VxFileAudioDataGenerator(vxFile,
        VxFileAudioDataGeneratorConfig()) {

}

int VxFileAudioDataGenerator::getSeek() const {
    return seek;
}

void VxFileAudioDataGenerator::setSeek(int seek) {
    this->seek = seek;
}

int VxFileAudioDataGenerator::getOutBufferSize() const {
    return outBufferSize;
}

int VxFileAudioDataGenerator::getSampleRate() const {
    return sampleRate;
}

double VxFileAudioDataGenerator::getDurationInSeconds() const {
    return vxFile.getDurationInSeconds();
}

const VxFile &VxFileAudioDataGenerator::getVxFile() const {
    return vxFile;
}

void VxFileAudioDataGenerator::setVxFile(const VxFile &vxFile) {
    this->vxFile = vxFile;
}

VxFileAudioDataGenerator::~VxFileAudioDataGenerator() {
    tsf_close(_tsf);
}
