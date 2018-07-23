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

#define SEEK_LOCK std::lock_guard<std::mutex> _(seekMutex)
#define VXFILE_LOCK std::lock_guard<std::mutex> _(vxFileMutex)

int VxFileAudioDataGenerator::readNextSamplesBatch(short *intoBuffer, bool moveSeekAndFillWithZero) {
    int seek = getSeek();
    int size = std::min(pcmDataSize - seek, outBufferSize);

    if (!moveSeekAndFillWithZero) {
        double startTime = GetSampleTimeInSeconds(seek, sampleRate);
        double endTime = GetSampleTimeInSeconds(seek + size, sampleRate);

        VxFile vxFile = getVxFile();

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

        pitchesIndexes = std::move(tempPitchIndexes);
    } else {
        Memory::FillZero(intoBuffer, size);
    }

    {
        SEEK_LOCK;
        if (seek == this->seek) {
            this->seek += size;
        }
    }

    return size;
}

VxFileAudioDataGenerator::VxFileAudioDataGenerator(const VxFileAudioDataGeneratorConfig &config)
        : outBufferSize(config.outBufferSize), sampleRate(config.sampleRate) {
    pitchesIndexes.reserve(10);
    tempPitchIndexes.reserve(10);
    difference.reserve(10);

    _tsf = LoadTsf();
    tsf_set_output(_tsf, TSF_MONO, sampleRate, 0);
}

VxFileAudioDataGenerator::VxFileAudioDataGenerator() : VxFileAudioDataGenerator(VxFileAudioDataGeneratorConfig()) {

}

int VxFileAudioDataGenerator::getSeek() const {
    SEEK_LOCK;
    return seek;
}

void VxFileAudioDataGenerator::setSeek(int seek) {
    SEEK_LOCK;
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
    VXFILE_LOCK;
    return vxFile;
}

void VxFileAudioDataGenerator::setVxFile(const VxFile &vxFile) {
    VXFILE_LOCK;
    assert(this->vxFile.getDurationInTicks() == vxFile.getDurationInTicks() &&
            this->vxFile.getTicksPerSecond() == vxFile.getTicksPerSecond());
    this->vxFile = vxFile;
}

void VxFileAudioDataGenerator::resetVxFile(const VxFile &vxFile) {
    setSeek(0);
    double durationInSeconds = vxFile.getDurationInSeconds();
    pcmDataSize = (int)ceil(durationInSeconds * sampleRate);
    VXFILE_LOCK;
    this->vxFile = vxFile;
}

VxFileAudioDataGenerator::~VxFileAudioDataGenerator() {
    tsf_close(_tsf);
}
