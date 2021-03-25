//
// Created by Semyon Tikhonenko on 7/20/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <cmath>
#include "VocalPartAudioDataGenerator.h"
#include "AudioUtils.h"
#include <iostream>
#include "MemoryUtils.h"
#include "Sets.h"
#include "StringUtils.h"
#include "Primitives.h"
#include <iostream>

using namespace CppUtils;
using namespace AudioUtils;

using std::cout;
using std::endl;

#define SEEK_LOCK std::lock_guard<std::mutex> _(seekMutex)
#define VXFILE_LOCK std::lock_guard<std::mutex> _(vxFileMutex)

void VocalPartAudioDataGenerator::onPitches(const std::vector<int> &indexes) {
    iteratePitches(indexes, [&] (const Pitch& pitch) {
        pitchRenderer->on(pitch);
    });
}

void VocalPartAudioDataGenerator::offPitches(const std::vector<int> &indexes) {
    iteratePitches(indexes, [&] (const Pitch& pitch) {
        pitchRenderer->off(pitch);
    });
}

int VocalPartAudioDataGenerator::readNextSamplesBatch(short *intoBuffer, bool moveSeekAndFillWithZero) {
    const VocalPart& vocalPart = getVocalPart();
    int seek = getSeek();
    int framesCount = std::min(pcmDataSamplesCount - seek, playbackData.samplesPerBuffer);

    {
        VXFILE_LOCK;
        if (requestOffPitches) {
            offPitches(pitchesIndexes);
            pitchesIndexes.clear();
            requestOffPitches = false;
        }
    }

    if (!moveSeekAndFillWithZero) {
        double startTime = GetSampleTimeInSeconds(seek, playbackData.sampleRate);
        double endTime = GetSampleTimeInSeconds(seek + framesCount, playbackData.sampleRate);

        tempPitchIndexes.clear();
        vocalPart.getPitchesIndexesInTimeRange(startTime, endTime, std::back_inserter(tempPitchIndexes));

        difference.clear();
        Sets::Difference(pitchesIndexes, tempPitchIndexes, std::back_inserter(difference));
        offPitches(difference);

        difference.clear();
        Sets::Difference(tempPitchIndexes, pitchesIndexes, std::back_inserter(difference));
        onPitches(difference);

        pitchRenderer->render(intoBuffer, framesCount);

        pitchesIndexes = std::move(tempPitchIndexes);
    } else {
        Memory::FillZero(intoBuffer, framesCount * playbackData.numberOfChannels);
    }

    {
        SEEK_LOCK;
        if (seek == this->seek) {
            this->seek += framesCount;
        }
    }

    return framesCount;
}

VocalPartAudioDataGenerator::VocalPartAudioDataGenerator(PitchRenderer* pitchRenderer)
        : pitchRenderer(pitchRenderer) {
    requestOffPitches = false;
    pitchesIndexes.reserve(10);
    tempPitchIndexes.reserve(10);
    difference.reserve(10);
}

void VocalPartAudioDataGenerator::init(const PlaybackData &config) {
    this->playbackData = config;
    pitchRenderer->init(config.sampleRate, config.numberOfChannels, config.samplesPerBuffer);
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
    return playbackData.samplesPerBuffer * playbackData.numberOfChannels;
}

int VocalPartAudioDataGenerator::getSampleRate() const {
    return playbackData.sampleRate;
}

double VocalPartAudioDataGenerator::getDurationInSeconds() const {
    return vocalPart.getDurationInSeconds();
}

const VocalPart &VocalPartAudioDataGenerator::getVocalPart() const {
    VXFILE_LOCK;
    return vocalPart;
}

void VocalPartAudioDataGenerator::prepareForVocalPartSet(const VocalPart& vocalPart) {
    double durationInSeconds = vocalPart.getDurationInSeconds();
    pcmDataSamplesCount = (int)ceil(durationInSeconds * playbackData.sampleRate);
    requestOffPitches = true;
}

void VocalPartAudioDataGenerator::setVocalPart(VocalPart &&vocalPart) {
    VXFILE_LOCK;
    prepareForVocalPartSet(vocalPart);
    this->vocalPart = std::move(vocalPart);
}

void VocalPartAudioDataGenerator::setVocalPart(const VocalPart &vocalPart) {
    VXFILE_LOCK;
    prepareForVocalPartSet(vocalPart);
    this->vocalPart = vocalPart;
}

VocalPartAudioDataGenerator::~VocalPartAudioDataGenerator() {
    delete pitchRenderer;
}
