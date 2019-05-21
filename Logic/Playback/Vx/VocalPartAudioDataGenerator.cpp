//
// Created by Semyon Tikhonenko on 7/20/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <cmath>
#include "VocalPartAudioDataGenerator.h"
#include "AudioUtils.h"
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

int VocalPartAudioDataGenerator::readNextSamplesBatch(short *intoBuffer, bool moveSeekAndFillWithZero) {
    assert(!vocalPart.isEmpty());
    int seek = getSeek();
    int size = std::min(pcmDataSize - seek, outBufferSize);

    if (!moveSeekAndFillWithZero) {
        double startTime = GetSampleTimeInSeconds(seek, sampleRate);
        double endTime = GetSampleTimeInSeconds(seek + size, sampleRate);

        VocalPart vxFile = getVocalPart();

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

VocalPartAudioDataGenerator::VocalPartAudioDataGenerator(const VocalPart& vocalPart, const VocalPartAudioDataGeneratorConfig &config)
        : outBufferSize(config.outBufferSize), sampleRate(config.sampleRate) {
    pitchesIndexes.reserve(10);
    tempPitchIndexes.reserve(10);
    difference.reserve(10);

    _tsf = LoadTsf();
    assert(_tsf);
    tsf_set_output(_tsf, TSF_MONO, sampleRate, 0);
    resetVocalPart(vocalPart);
}

VocalPartAudioDataGenerator::VocalPartAudioDataGenerator(const VocalPart& vocalPart) :
VocalPartAudioDataGenerator(vocalPart, VocalPartAudioDataGeneratorConfig()) {

}

int VocalPartAudioDataGenerator::getSeek() const {
    SEEK_LOCK;
    return seek;
}

void VocalPartAudioDataGenerator::setSeek(int seek) {
    SEEK_LOCK;
    this->seek = seek;
}

int VocalPartAudioDataGenerator::getOutBufferSize() const {
    return outBufferSize;
}

int VocalPartAudioDataGenerator::getSampleRate() const {
    return sampleRate;
}

double VocalPartAudioDataGenerator::getDurationInSeconds() const {
    return vocalPart.getDurationInSeconds();
}

const VocalPart &VocalPartAudioDataGenerator::getVocalPart() const {
    VXFILE_LOCK;
    return vocalPart;
}

void VocalPartAudioDataGenerator::setVocalPart(const VocalPart &vocalPart) {
    VXFILE_LOCK;
    assert(this->vocalPart.getDurationInTicks() == vocalPart.getDurationInTicks() &&
            this->vocalPart.getTicksPerSecond() == vocalPart.getTicksPerSecond() &&
            "Use resetVocalPart instead");
    this->vocalPart = vocalPart;
}

void VocalPartAudioDataGenerator::resetVocalPart(const VocalPart &vocalPart) {
    setSeek(0);
    double durationInSeconds = vocalPart.getDurationInSeconds();
    pcmDataSize = (int)ceil(durationInSeconds * sampleRate);
    VXFILE_LOCK;
    this->vocalPart = vocalPart;
}

VocalPartAudioDataGenerator::~VocalPartAudioDataGenerator() {
    tsf_close(_tsf);
}

std::vector<short> VocalPartAudioDataGenerator::readAll() {
    assert(!vocalPart.isEmpty());
    std::vector<short> result;
    result.reserve(size_t(pcmDataSize));

    std::vector<short> temp(static_cast<size_t>(outBufferSize));
    int size = 0;
    do {
        size = readNextSamplesBatch(temp.data());
        result.insert(result.end(), temp.begin(), temp.begin() + size);
    } while (size == outBufferSize);

    return result;
}

VocalPartAudioDataGenerator::VocalPartAudioDataGenerator() : VocalPartAudioDataGenerator(VocalPart()) {

}
