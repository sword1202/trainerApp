//
// Created by Semyon Tikhonenko on 4/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <cmath>
#include <boost/assert.hpp>
#include "VxFileAudioDataGenerator.h"
#include "AudioUtils.h"
#include "Algorithms.h"
#include "SoundFont2PitchRenderer.h"
#include <iostream>

#define BUFFER_LOCK std::lock_guard<std::mutex> _(bufferReadingMutex)
#define SEEK_LOCK std::lock_guard<std::mutex> _(seekMutex)

using namespace boost::icl;
using namespace CppUtils;
using namespace StlDebugUtils;
using std::cout;

constexpr double PITCH_EDGE_SMOOTH_FACTOR = 0.07 * 400;

VxFileAudioDataGenerator::VxFileAudioDataGenerator(PitchRenderer *renderer, VxFile &&vxFile, const VxFileAudioDataGeneratorConfig &config)
        : renderer(renderer), vxFile(std::move(vxFile)) {
    sampleRate = config.sampleRate;
    outBufferSize = config.outBufferSize;
    BOOST_ASSERT(sampleRate > 0 && outBufferSize > 0);

    double durationInSeconds = vxFile.getDurationInSeconds();
    int pcmDataSize = (int)ceil(durationInSeconds * sampleRate);
    pcmData.assign(pcmDataSize, 0);
    fullyInitializedPcmData.assign(pcmDataSize, 0);
    pcmHasDataFlag.resize(pcmDataSize, false);
}


VxFileAudioDataGenerator::VxFileAudioDataGenerator(PitchRenderer *renderer, VxFile &&vxFile)
: VxFileAudioDataGenerator(renderer, (VxFile&&)vxFile, VxFileAudioDataGeneratorConfig())
{

}

VxFileAudioDataGenerator::VxFileAudioDataGenerator(const VxFile &vxFile)
    : VxFileAudioDataGenerator(renderer, std::move(VxFile(vxFile)), VxFileAudioDataGeneratorConfig()) {

}


VxFileAudioDataGenerator::VxFileAudioDataGenerator(VxFile &&vxFile, const VxFileAudioDataGeneratorConfig &config) :
    VxFileAudioDataGenerator(nullptr, (VxFile&&)vxFile, config) {
    int ticksPerSecond = vxFile.getTicksPerSecond();
    renderer = new SoundFont2PitchRenderer(config.sampleRate, PITCH_EDGE_SMOOTH_FACTOR / ticksPerSecond);
}

VxFileAudioDataGenerator::VxFileAudioDataGenerator(VxFile &&vxFile)  :
        VxFileAudioDataGenerator(std::move(vxFile), VxFileAudioDataGeneratorConfig()) {
}

void VxFileAudioDataGenerator::clearAllData() {
    std::fill(begin(pcmData), end(pcmData), 0);
    std::fill(begin(pcmHasDataFlag), end(pcmHasDataFlag), false);
    renderedPitchesIndexes.clear();
    BUFFER_LOCK;
    fullyInitializedDataIntervals.clear();
    std::fill(begin(fullyInitializedPcmData), end(fullyInitializedPcmData), 0);
    renderedPitchesCount = 0;
}

bool VxFileAudioDataGenerator::renderNextPitchIfPossible() {
    const std::vector<VxPitch> &pitches = vxFile.getPitches();
    int pitchToRenderIndex = getNextPitchToRenderIndex();
    if (pitchToRenderIndex < 0) {
        return false;
    }

    const VxPitch &pitch = pitches[pitchToRenderIndex];

    int first = vxFile.samplesCountFromTicks(pitch.startTickNumber, sampleRate);
    int length = vxFile.samplesCountFromTicks(pitch.ticksCount, sampleRate);
    if (first + length > pcmData.size()) {
        length = pcmData.size() - first;
    }

    renderPitch(pitch.pitch, first, length);
    
    int renderedDataSize = 0;
    if (pitchToRenderIndex >= pitches.size() - 1) {
        renderedDataSize = pcmData.size() - first;
    } else {
        const VxPitch &nextPitch = pitches[pitchToRenderIndex + 1];
        renderedDataSize = vxFile.samplesCountFromTicks(nextPitch.startTickNumber, sampleRate) - first;
    }

    renderedPitchesIndexes.insert(pitchToRenderIndex);

    BUFFER_LOCK;
    // add silence at track start on the beginning
    if (pitchToRenderIndex == 0) {
        renderedDataSize += first;
        first = 0;
    }

    fullyInitializedDataIntervals.add(interval<int>::right_open(first, first + renderedDataSize));
    auto iter = begin(pcmData) + first;
    std::copy(iter, iter + renderedDataSize, begin(fullyInitializedPcmData) + first);

    return true;
}

VxFileAudioDataGenerator::~VxFileAudioDataGenerator() {
    delete renderer;
}

int VxFileAudioDataGenerator::readNextSamplesBatch(short *intoBuffer) {
    int seek;
    {
        SEEK_LOCK;
        seek = this->seek;
    }

    if (!isFullyInitialized(seek, seek + outBufferSize)) {
        return -1;
    }

    size_t size = std::min(pcmData.size() - seek, (size_t)outBufferSize);
    auto first = begin(fullyInitializedPcmData) + seek;

    {
        BUFFER_LOCK;
        std::copy(first, first + size, intoBuffer);
    }

    {
        SEEK_LOCK;
        int lastSeek = this->seek;
        if (lastSeek == seek) {
            this->seek += size;
        }
    }

    return (int)size;
}

bool VxFileAudioDataGenerator::isFullyInitialized(int begin, int end) const {
    return contains(fullyInitializedDataIntervals, interval<int>::right_open(begin, end));
}

int VxFileAudioDataGenerator::getNextPitchToRenderIndex() const {
    const std::vector<VxPitch> &pitches = vxFile.getPitches();
    if (renderedPitchesIndexes.size() == pitches.size()) {
        return -1;
    }

    int seekInTicks;
    {
        SEEK_LOCK;
        seekInTicks = vxFile.ticksFromSamplesCount(seek, sampleRate);
    }
    VxPitch stub;
    stub.startTickNumber = seekInTicks;
    auto iter = UpperBoundByKey(pitches, stub, VxFile::startTickNumberKeyProvider) - 1;
    if (iter == begin(pitches) - 1 || iter->endTickNumber() <= seekInTicks) {
        iter++;
    }

    int index = iter - begin(pitches);

    auto i = renderedPitchesIndexes.find(index);
    if (i != renderedPitchesIndexes.end()) {
        do {
            index++;
            i++;
        } while (i != renderedPitchesIndexes.end() && *i == index);
    }

    if (index >= pitches.size()) {
        return -1;
    }

    return index;
}

void VxFileAudioDataGenerator::renderAllData() {
    std::fill(begin(pcmData), end(pcmData), 0);

    const std::vector<VxPitch> &pitches = vxFile.getPitches();
    int pitchesSize = pitches.size();
    for (int i = 0; i < pitchesSize; ++i) {
       renderedPitchesIndexes.insert(i);
    }

    for (const VxPitch& vxPitch : pitches) {
        int begin = vxFile.samplesCountFromTicks(vxPitch.startTickNumber, sampleRate);
        int length = vxFile.samplesCountFromTicks(vxPitch.ticksCount, sampleRate);

        renderPitch(vxPitch.pitch, begin, length);
    }

    std::fill(begin(pcmHasDataFlag), end(pcmHasDataFlag), true);
    BUFFER_LOCK;
    fullyInitializedDataIntervals.add(interval<int>::right_open(0, pcmData.size()));
    fullyInitializedPcmData = pcmData;
}

void VxFileAudioDataGenerator::renderPitch(const Pitch &pitch, int begin, int length) {
    BOOST_ASSERT(begin >= 0 && length >= 0 && length + begin <= pcmData.size());

    std::vector<short> temp(length);
    renderer->render(pitch, temp.data(), length);

    for (int i = 0; i < length; ++i) {
        short value = temp[i];
        if (pcmHasDataFlag[i + begin]) {
            int pcmValue = pcmData[i + begin];
            pcmData[i + begin] = (short)((pcmValue + value) / 2);
        } else {
            pcmData[i + begin] = value;
            pcmHasDataFlag[i + begin] = true;
        }
    }

    renderedPitchesCount++;
    BOOST_ASSERT(renderedPitchesCount <= vxFile.getPitches().size());
}

int VxFileAudioDataGenerator::getTotalSamplesCount() const {
    return pcmData.size();
}

int VxFileAudioDataGenerator::getOutBufferSize() const {
    return outBufferSize;
}

int VxFileAudioDataGenerator::getSampleRate() const {
    return sampleRate;
}

void VxFileAudioDataGenerator::setSeek(int seek) {
    SEEK_LOCK;
    this->seek = seek;
}

int VxFileAudioDataGenerator::getSeek() const {
    SEEK_LOCK;
    return seek;
}

double VxFileAudioDataGenerator::getDurationInSeconds() const {
    return vxFile.getDurationInSeconds();
}
