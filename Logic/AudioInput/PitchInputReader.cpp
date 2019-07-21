//
// Created by Semyon Tikhonenko on 8/7/16.
//

#include <cstdlib>
#include <iostream>
#include "PitchInputReader.h"
#include "MemoryUtils.h"

using namespace CppUtils;

#define LOCK std::lock_guard<std::mutex> _(mutex)

void PitchInputReader::operator()(const int16_t* buffer, int size) {
    buffer = smoothingAudioBuffer.getRunPitchDetectionBufferIfReady(buffer, (size_t) size);
    if (!buffer) {
        return;
    }

    auto* bufferCopy = new std::vector<int16_t>(buffer, buffer + size);
    threadPool->push([=] (int id) {
        const int16_t* bufferPtr = bufferCopy->data();

        PitchDetector* pitchDetector = pitchDetectors[id];
        float frequency = pitchDetector->getFrequencyFromBuffer(bufferPtr);
        if (frequency > 0) {
            Pitch lastDetectedPitch(frequency);
            if (callback) {
                callback(lastDetectedPitch);
            }

            {
                LOCK;
                this->lastDetectedPitch = lastDetectedPitch;
            }

        } else if(executeCallBackOnInvalidPitches && callback) {
            callback(Pitch(frequency));
        }

        delete bufferCopy;
    });
}

PitchInputReader::PitchInputReader(AudioInputReader* audioInputReader,
        const std::function<PitchDetector*()>& pitchDetectorFactory, int smoothLevel) :
        pitchDetectorFactory(pitchDetectorFactory),
        smoothingAudioBuffer((size_t) smoothLevel, (size_t) audioInputReader->getMaximumBufferSize()) {
    int sampleRate = audioInputReader->getSampleRate();
    int cpuCount = std::thread::hardware_concurrency();
    if (cpuCount == 0) {
        cpuCount = 2;
    }

    pitchDetectors.resize(cpuCount);
    for (int i = 0; i < cpuCount; ++i) {
        PitchDetector* pitchDetector = pitchDetectors[i] = pitchDetectorFactory();
        pitchDetector->init(audioInputReader->getMaximumBufferSize() * smoothLevel, sampleRate);
    }

    threadPool = new ctpl::thread_pool(cpuCount);
}

PitchInputReader::~PitchInputReader() {
    delete threadPool;
    Memory::DeleteAll(pitchDetectors);
}

void PitchInputReader::setCallback(const std::function<void(Pitch)>& callback) {
    this->callback = callback;
}

const Pitch &PitchInputReader::getLastDetectedPitch() const {
    LOCK;
    return lastDetectedPitch;
}

bool PitchInputReader::willExecuteCallBackOnInvalidPitches() const {
    return executeCallBackOnInvalidPitches;
}

void PitchInputReader::setExecuteCallBackOnInvalidPitches(bool executeCallBackOnInvalidPitches) {
    PitchInputReader::executeCallBackOnInvalidPitches = executeCallBackOnInvalidPitches;
}
