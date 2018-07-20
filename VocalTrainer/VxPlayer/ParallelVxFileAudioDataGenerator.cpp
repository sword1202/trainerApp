//
// Created by Semyon Tikhonenko on 4/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <cmath>
#include <boost/assert.hpp>
#include "ParallelVxFileAudioDataGenerator.h"
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

ParallelVxFileAudioDataGenerator::ParallelVxFileAudioDataGenerator(PitchRenderer *renderer, const VxFile &vxFile,
        const VxFileAudioDataGeneratorConfig &config)
        : renderer(renderer), vxFile(std::move(vxFile)) {
    sampleRate = config.sampleRate;
    outBufferSize = config.outBufferSize;
    BOOST_ASSERT(sampleRate > 0 && outBufferSize > 0);

    double durationInSeconds = vxFile.getDurationInSeconds();
    int pcmDataSize = (int)ceil(durationInSeconds * sampleRate);
    pcmData.assign(pcmDataSize, 0);
    publishedPcmData.assign(pcmDataSize, 0);
    summarizedPcmData.resize(pcmDataSize, 0);
    divisionFactor.resize(pcmDataSize, 0);

    resetPublishedDataIntervals();
}

void ParallelVxFileAudioDataGenerator::resetPublishedDataIntervals() {
    publishedDataIntervals.add(interval<int>::right_open(0, getFullyFilledPcmDataSize()));
    for (const auto& pitch : vxFile.getPitches()) {
        int start = vxFile.samplesCountFromTicks(pitch.startTickNumber, sampleRate);
        int end = vxFile.samplesCountFromTicks(pitch.endTickNumber(), sampleRate);
        publishedDataIntervals.subtract(interval<int>::right_open(start, end));
    }
}


ParallelVxFileAudioDataGenerator::ParallelVxFileAudioDataGenerator(PitchRenderer *renderer, const VxFile &vxFile)
: ParallelVxFileAudioDataGenerator(renderer, vxFile, VxFileAudioDataGeneratorConfig())
{

}

ParallelVxFileAudioDataGenerator::ParallelVxFileAudioDataGenerator(const VxFile &vxFile)
    : ParallelVxFileAudioDataGenerator(vxFile, VxFileAudioDataGeneratorConfig()) {
}


ParallelVxFileAudioDataGenerator::ParallelVxFileAudioDataGenerator(const VxFile &vxFile, const VxFileAudioDataGeneratorConfig &config) :
    ParallelVxFileAudioDataGenerator(nullptr, vxFile, config) {
    int ticksPerSecond = vxFile.getTicksPerSecond();
    renderer = new SoundFont2PitchRenderer(config.sampleRate, PITCH_EDGE_SMOOTH_FACTOR / ticksPerSecond);
}

void ParallelVxFileAudioDataGenerator::clearAllData() {
    std::fill(begin(pcmData), end(pcmData), 0);
    std::fill(begin(summarizedPcmData), end(summarizedPcmData), 0);
    std::fill(begin(divisionFactor), end(divisionFactor), 0);
    renderedPitchesIndexes.clear();
    publishedPitchesIndexes.clear();
    BUFFER_LOCK;
    publishedDataIntervals.clear();
    resetPublishedDataIntervals();
    std::fill(begin(publishedPcmData), end(publishedPcmData), 0);
}


void ParallelVxFileAudioDataGenerator::publishPitchIfFullyRendered(int index) {
    const auto& pitches = vxFile.getPitches();
    const VxPitch vxPitch = pitches[index];

    std::vector<int> intersectedIndexes = FindIndexes(pitches, [&] (const VxPitch& pitch) {
        return pitch.intersectsWith(vxPitch);
    });

    if (std::all_of(intersectedIndexes.begin(), intersectedIndexes.end(), [=] (int index) {
        return renderedPitchesIndexes.count(index);
    })) {
        int first = vxFile.samplesCountFromTicks(vxPitch.startTickNumber, sampleRate);
        int length = vxFile.samplesCountFromTicks(vxPitch.ticksCount, sampleRate);

        auto iter = begin(pcmData) + first;
        for (int i = first; i < first + length; ++i) {
            pcmData[i] = (short)round((double)summarizedPcmData[i] / divisionFactor[i]);
        }

        {
            BUFFER_LOCK;
            publishedDataIntervals.add(interval<int>::right_open(first, first + length));
            std::copy(iter, iter + length, begin(publishedPcmData) + first);
        }

        publishedPitchesIndexes.insert(index);

        for (int intersectedIndex : intersectedIndexes) {
            if (!publishedPitchesIndexes.count(intersectedIndex)) {
                publishPitchIfFullyRendered(intersectedIndex);
            }
        }
    }
}

bool ParallelVxFileAudioDataGenerator::renderNextPitchIfPossible() {
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
    renderedPitchesIndexes.insert(pitchToRenderIndex);

    publishPitchIfFullyRendered(pitchToRenderIndex);

    return true;
}

ParallelVxFileAudioDataGenerator::~ParallelVxFileAudioDataGenerator() {
    delete renderer;
}

int ParallelVxFileAudioDataGenerator::readNextSamplesBatch(short *intoBuffer) {
    int seek;
    {
        SEEK_LOCK;
        seek = this->seek;
    }

    if (!isPublished(seek, seek + outBufferSize)) {
        return -1;
    }

    size_t size = std::min(pcmData.size() - seek, (size_t)outBufferSize);
    auto first = begin(publishedPcmData) + seek;

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

bool ParallelVxFileAudioDataGenerator::isPublished(int begin, int end) const {
    if (end > pcmData.size()) {
        end = pcmData.size();
    }

    BUFFER_LOCK;
    return contains(publishedDataIntervals, interval<int>::right_open(begin, end));
}

int ParallelVxFileAudioDataGenerator::getNextPitchToRenderIndex() const {
    const std::vector<VxPitch> &pitches = vxFile.getPitches();
    if (renderedPitchesIndexes.size() == pitches.size()) {
        return -1;
    }

    int seekInTicks;
    {
        SEEK_LOCK;
        seekInTicks = vxFile.ticksFromSamplesCount(seek, sampleRate);
    }

    int index = FindIndex(pitches, [=] (const VxPitch& vxPitch) {
        return vxPitch.containsTick(seekInTicks);
    });

    if (index < 0) {
        VxPitch stub;
        stub.startTickNumber = seekInTicks;
        auto iter = UpperBoundByKey(pitches, stub, VxFile::startTickNumberKeyProvider);
        index = iter - begin(pitches);
    }

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

void ParallelVxFileAudioDataGenerator::renderAllData() {
    assert("Doesn't work" && false);
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

    BUFFER_LOCK;
    publishedDataIntervals.add(interval<int>::right_open(0, pcmData.size()));
    publishedPcmData = pcmData;
}

void ParallelVxFileAudioDataGenerator::renderPitch(const Pitch &pitch, int begin, int length) {
    BOOST_ASSERT(begin >= 0 && length >= 0 && length + begin <= pcmData.size());

    std::vector<short> temp(length);
    renderer->render(pitch, temp.data(), length);

    for (int i = 0; i < length; ++i) {
        short value = temp[i];
        summarizedPcmData[i + begin] += value;
        divisionFactor[i + begin]++;
    }
}

int ParallelVxFileAudioDataGenerator::getTotalSamplesCount() const {
    return pcmData.size();
}

int ParallelVxFileAudioDataGenerator::getOutBufferSize() const {
    return outBufferSize;
}

int ParallelVxFileAudioDataGenerator::getSampleRate() const {
    return sampleRate;
}

void ParallelVxFileAudioDataGenerator::setSeek(int seek) {
    SEEK_LOCK;
    this->seek = seek;
}

int ParallelVxFileAudioDataGenerator::getSeek() const {
    SEEK_LOCK;
    return seek;
}

double ParallelVxFileAudioDataGenerator::getDurationInSeconds() const {
    return vxFile.getDurationInSeconds();
}

void ParallelVxFileAudioDataGenerator::setVxFile(const VxFile &vxFile) {
    this->vxFile = vxFile;
    clearAllData();
}

const VxFile &ParallelVxFileAudioDataGenerator::getVxFile() const {
    return vxFile;
}

int ParallelVxFileAudioDataGenerator::getFullyFilledPcmDataSize() const {
    return pcmData.size();
}
