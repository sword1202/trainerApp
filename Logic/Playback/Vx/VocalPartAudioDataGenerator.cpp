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
    int framesCount = std::min(pcmDataSamplesCount - seek, playbackData.samplesPerBuffer);

    if (!moveSeekAndFillWithZero) {
        double startTime = GetSampleTimeInSeconds(seek, playbackData.sampleRate);
        double endTime = GetSampleTimeInSeconds(seek + framesCount, playbackData.sampleRate);

        const VocalPart& vocalPart = getVocalPart();

        tempPitchIndexes.clear();
        vocalPart.getPitchesIndexesInTimeRange(startTime, endTime, std::back_inserter(tempPitchIndexes));

        difference.clear();
        Sets::Difference(pitchesIndexes, tempPitchIndexes, std::back_inserter(difference));
        for (int pitchIndex : difference) {
            const Pitch& pitch = vocalPart.getNotes()[pitchIndex].pitch;
            pitchRenderer->off(pitch);
        }

        difference.clear();
        Sets::Difference(tempPitchIndexes, pitchesIndexes, std::back_inserter(difference));
        for (int pitchIndex : difference) {
            const Pitch& pitch = vocalPart.getNotes()[pitchIndex].pitch;
            pitchRenderer->on(pitch);
        }

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
    pcmDataSamplesCount = (int)ceil(durationInSeconds * playbackData.sampleRate);
    VXFILE_LOCK;
    this->vocalPart = vocalPart;
}

VocalPartAudioDataGenerator::~VocalPartAudioDataGenerator() {
    delete pitchRenderer;
}
