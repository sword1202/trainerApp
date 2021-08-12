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
#include "MathUtils.h"
#include <iostream>
#include <unordered_set>

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
            offPitches(onnedPitches);
            onnedPitches.clear();
            requestOffPitches = false;
        }
    }

    if (framesCount == 0) {
        return 0;
    }

    if (!moveSeekAndFillWithZero) {
        VXFILE_LOCK;
        if (batchPlayingPitchesMap.empty()) {
            Memory::FillZero(intoBuffer, framesCount * playbackData.numberOfChannels);
        } else {
            int batchIndex = seek / playbackData.samplesPerBuffer;
            auto iter = batchPlayingPitchesMap.find(batchIndex);
            if (iter == batchPlayingPitchesMap.end()) {
                offPitches(onnedPitches);
                onnedPitches.clear();
            } else {
                Sets::Difference(onnedPitches, iter->second, std::back_inserter(difference));
                offPitches(difference);

                difference.clear();
                Sets::Difference(iter->second, onnedPitches, std::back_inserter(difference));
                onPitches(difference);
                difference.clear();

                onnedPitches = iter->second;
            }

            pitchRenderer->render(intoBuffer, framesCount);
        }
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
    onnedPitches.reserve(10);
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
    VXFILE_LOCK;
    return vocalPart.getDurationInSeconds();
}

const VocalPart &VocalPartAudioDataGenerator::getVocalPart() const {
    VXFILE_LOCK;
    return vocalPart;
}

void VocalPartAudioDataGenerator::prepareForVocalPartSet(const VocalPart& vocalPart) {
    double newDuration = vocalPart.getDurationInSeconds();
    double currentDuration = this->vocalPart.getDurationInSeconds();
    if (!Primitives::CompareFloatsUsingEpsilon(newDuration, currentDuration, 0.000001)) {
        SEEK_LOCK;
        seek = Math::RoundToInt(seek * newDuration / currentDuration);
    }
    pcmDataSamplesCount = (int)ceil(newDuration * playbackData.sampleRate);
    int batchSamplesCount = playbackData.samplesPerBuffer;
    const auto &notes = vocalPart.getNotes();
    batchPlayingPitchesMap.clear();

    std::unordered_set<int> onBatchIndexes;
    std::unordered_set<int> offBatchIndexes;
    for (int noteIndex = 0; noteIndex < notes.size(); ++noteIndex) {
        const NoteInterval& note = notes[noteIndex];
        int onSampleIndex = vocalPart.samplesCountFromTicks(note.startTickNumber, playbackData.sampleRate);
        int onBatchIndex = onSampleIndex / batchSamplesCount;
        int offSampleIndex = vocalPart.samplesCountFromTicks(note.endTickNumber(), playbackData.sampleRate);
        int offBatchIndex = offSampleIndex / batchSamplesCount;
        if (onBatchIndex == offBatchIndex) {
            offBatchIndex++;
        }

        // Prevent a situation, when a batch has on and off at the same time
        while (offBatchIndexes.count(onBatchIndex)) {
            onBatchIndex++;
        }

        while (onBatchIndexes.count(offBatchIndex)) {
            offBatchIndex--;
        }

        for (int batchIndex = onBatchIndex; batchIndex < offBatchIndex; ++batchIndex) {
            batchPlayingPitchesMap[batchIndex].push_back(noteIndex);
        }

        // Check if the note is valid
        if (onBatchIndex < offBatchIndex) {
            onBatchIndexes.insert(onBatchIndex);
            offBatchIndexes.insert(offBatchIndex);
        }
    }

    for (auto& entry : batchPlayingPitchesMap) {
        std::sort(entry.second.begin(), entry.second.end());
    }
    
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
