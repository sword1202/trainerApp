//
// Created by Semyon Tikhonenko on 7/20/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <cmath>
#include "VxFileAudioDataGenerator.h"
#include "AudioUtils.h"
#include "SoundFont2PitchRenderer.h"
#include <iostream>

using namespace CppUtils;
using namespace AudioUtils;

int VxFileAudioDataGenerator::readNextSamplesBatch(short *intoBuffer) {
    double startTime = GetSampleTimeInSeconds(seek, sampleRate);
    double endTime = GetSampleTimeInSeconds(seek + outBufferSize, sampleRate);
    tempPitches.clear();
    vxFile.getPitchesInTimeRange(startTime, endTime, std::back_inserter(tempPitches));

    int size = std::min(pcmDataSize - seek, outBufferSize);

    if (tempPitches.empty()) {
        return size;
    }

    std::fill(renderedPitchesSummary.begin(), renderedPitchesSummary.end(), 0);
    std::fill(overlappingCountMap.begin(), overlappingCountMap.end(), 0);

    int bufferStartTick = vxFile.ticksFromSamplesCount(seek, sampleRate);
    int bufferEndTick = vxFile.ticksFromSamplesCount(seek + size, sampleRate);

    for (const VxPitch& vxPitch : tempPitches) {
        int startTick = std::max(bufferStartTick, vxPitch.startTickNumber);
        int endTick = std::min(bufferEndTick, vxPitch.endTickNumber());

        int start = vxFile.samplesCountFromTicks(startTick, sampleRate) - seek;
        assert(start >= 0);
        int end = vxFile.samplesCountFromTicks(endTick, sampleRate) - seek;
        assert(end <= size);

        renderer->render(vxPitch.pitch, temp.data() + start, end - start);

        for (int i = start; i < end; ++i) {
            ++overlappingCountMap[i];
            renderedPitchesSummary[i] += temp[i];
        }
    }

    for (int i = 0; i < size; ++i) {
        int overlappingCount = overlappingCountMap[i];
        if (overlappingCount == 0) {
            intoBuffer[i] = 0;
        } else {
            intoBuffer[i] = (short)round((double)renderedPitchesSummary[i] / overlappingCount);
        }
    }

    return size;
}

VxFileAudioDataGenerator::VxFileAudioDataGenerator(const VxFile& vxFile, PitchRenderer *renderer, const VxFileAudioDataGeneratorConfig &config)
        : outBufferSize(config.outBufferSize), sampleRate(config.sampleRate), renderer(renderer), vxFile(vxFile) {
    renderedPitchesSummary.resize((size_t)outBufferSize);
    temp.resize((size_t)outBufferSize);
    tempPitches.reserve(10);

    double durationInSeconds = vxFile.getDurationInSeconds();
    pcmDataSize = (int)ceil(durationInSeconds * sampleRate);
}

VxFileAudioDataGenerator::VxFileAudioDataGenerator(const VxFile& vxFile, PitchRenderer *renderer) :
        VxFileAudioDataGenerator(vxFile, renderer, VxFileAudioDataGeneratorConfig()) {

}

VxFileAudioDataGenerator::VxFileAudioDataGenerator(const VxFile &vxFile) : VxFileAudioDataGenerator(vxFile, nullptr) {
    renderer = new SoundFont2PitchRenderer(sampleRate);
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
    delete renderer;
}
