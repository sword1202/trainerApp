//
// Created by Semyon Tikhonenko on 4/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <cmath>
#include <boost/assert.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>
#include "VxFileAudioDataGenerator.h"
#include "AudioUtils.h"
#include "Algorithms.h"
#include "SoundFont2PitchRenderer.h"

using namespace boost::algorithm;
using namespace CppUtils;

VxFileAudioDataGenerator::VxFileAudioDataGenerator(PitchRenderer *renderer, const VxFile *vxFile, const VxFileAudioDataGeneratorConfig &config)
        : renderer(renderer) {
    sampleRate = config.sampleRate;
    outBufferSize = config.outBufferSize;
    BOOST_ASSERT(sampleRate > 0 && outBufferSize > 0);
    this->vxFile = vxFile;

    double durationInSeconds = vxFile->getDurationInSeconds();
    int pcmDataSize = (int)ceil(durationInSeconds * sampleRate);
    pcmData.assign(pcmDataSize, 0);
    pcmDataStateFlags.resize(pcmDataSize, NOT_INITIALIZED);
    bufferLengthInSeconds = AudioUtils::GetSampleTimeInSeconds(outBufferSize, sampleRate);
}

VxFileAudioDataGenerator::VxFileAudioDataGenerator(PitchRenderer *renderer, const VxFile *vxFile) :
        VxFileAudioDataGenerator(renderer, vxFile, VxFileAudioDataGeneratorConfig()) {
    
}

VxFileAudioDataGenerator::VxFileAudioDataGenerator(const VxFile *vxFile, const VxFileAudioDataGeneratorConfig &config) :
    VxFileAudioDataGenerator(new SoundFont2PitchRenderer(config.sampleRate), vxFile, config) {

}

VxFileAudioDataGenerator::VxFileAudioDataGenerator(const VxFile *vxFile)  :
        VxFileAudioDataGenerator(vxFile, VxFileAudioDataGeneratorConfig()) {
}

void VxFileAudioDataGenerator::reset() {
    seek = 0;
    std::fill(pcmData.begin(), pcmData.end(), 0);
    std::fill(pcmDataStateFlags.begin(), pcmDataStateFlags.end(), NOT_INITIALIZED);
}

void VxFileAudioDataGenerator::renderNextPitch() {
    const std::vector<VxPitch> &pitches = vxFile->getPitches();
    BOOST_ASSERT(canRenderNextPitch());
    int nextPitchToRenderIndex = getNextPitchToRenderIndex();
    const VxPitch &pitch = pitches[nextPitchToRenderIndex];

    // render a beat more to avoid floating computation errors;
    int begin = vxFile->samplesCountFromTicks(pitch.startTickNumber, sampleRate) - 1;
    int length = vxFile->samplesCountFromTicks(pitch.ticksCount, sampleRate) + 2;
    if (begin + length > pcmData.size()) {
        length = pcmData.size() - begin;
    }

    std::vector<short> temp(length);
    renderer->render(pitch.pitch, temp.data(), length);

    for (int i = 0; i < length; ++i) {
        short value = temp[i];
        if (pcmDataStateFlags[i + begin] != NOT_INITIALIZED) {
            int pcmValue = pcmData[i + begin];
            pcmData[i + begin] = (short)((pcmValue + value) / 2);
        } else {
            pcmData[i + begin] = value;
            pcmDataStateFlags[i + begin] = PARTLY_INITIALIZED;
        }
    }
    
    int renderedDataSize = 0;
    if (nextPitchToRenderIndex >= pitches.size() - 1) {
        renderedDataSize = pcmData.size() - begin;
    } else {
        const VxPitch &nextPitch = pitches[nextPitchToRenderIndex + 1];
        renderedDataSize = vxFile->samplesCountFromTicks(nextPitch.startTickNumber, sampleRate) - begin;
    }

    renderedPitchesIndexes.insert(nextPitchToRenderIndex);
    std::fill(pcmDataStateFlags.begin() + begin,
            pcmDataStateFlags.begin() + begin + renderedDataSize,
            FULLY_INITIALIZED);
}

VxFileAudioDataGenerator::~VxFileAudioDataGenerator() {
    delete renderer;
}

bool VxFileAudioDataGenerator::canRenderNextPitch() const {
    return vxFile->getPitches().back().endTickNumber();
}

int VxFileAudioDataGenerator::readNextSamplesBatch(short *intoBuffer) {
    while (!isFullyInitialized(seek, seek + outBufferSize) && canRenderNextPitch()) {
        renderNextPitch();
    }

    size_t size = std::min(pcmData.size() - seek, (size_t)outBufferSize);
    std::copy(pcmData.begin() + seek, pcmData.begin() + seek + size, intoBuffer);
    seek += size;
    return (int)size;
}

bool VxFileAudioDataGenerator::isFullyInitialized(int begin, int end) const {
    auto iter = pcmDataStateFlags.begin();
    return all_of_equal(iter + begin, iter + end, FULLY_INITIALIZED);
}

int VxFileAudioDataGenerator::getNextPitchToRenderIndex() const {
    int seekInTicks = vxFile->ticksFromSamplesCount(seek, sampleRate);
    VxPitch stub;
    stub.startTickNumber = seekInTicks;
    const std::vector<VxPitch> &pitches = vxFile->getPitches();
    auto iter = UpperBoundByKey(pitches, stub, VxFile::startTickNumberKeyProvider) - 1;
    if (iter == pitches.begin() - 1 || iter->endTickNumber() <= seekInTicks) {
        iter++;
    }

    int index = iter - pitches.begin();
    BOOST_ASSERT(index < pitches.size());

    auto i = renderedPitchesIndexes.find(index);
    if (i == renderedPitchesIndexes.end()) {
        return index;
    }
    
    do {
        index++;
        i++;
    } while (i != renderedPitchesIndexes.end() && *i == index);

    return index;
}
