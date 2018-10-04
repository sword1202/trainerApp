//
// Created by Semyon Tikhonenko on 8/7/16.
//

#include <cstdlib>
#include <iostream>
#include "PitchInputReader.h"

using namespace CppUtils;

void PitchInputReader::operator()(const int16_t* buffer, int size) {
    buffer = smoothingAudioBuffer.getRunPitchDetectionBufferIfReady(buffer, (size_t) size);
    if (!buffer) {
        return;
    }

    float frequency = pitchDetector->getFrequencyFromBuffer(buffer);
    if (frequency > 0) {
        lastDetectedPitch = Pitch(frequency);
        if (callback) {
            callback(lastDetectedPitch);
        }
    } else if(executeCallBackOnInvalidPitches && callback) {
        callback(Pitch(frequency));
    }
}

PitchInputReader::PitchInputReader(AudioInputReader* audioInputReader, PitchDetector* pitchDetector, int smoothLevel) :
        pitchDetector(pitchDetector),
        smoothingAudioBuffer((size_t) smoothLevel, (size_t) audioInputReader->getMaximumBufferSize()) {
    int sampleRate = audioInputReader->getSampleRate();
    pitchDetector->init(audioInputReader->getMaximumBufferSize() * smoothLevel, sampleRate);
}

PitchInputReader::~PitchInputReader() {
}

void PitchInputReader::setCallback(const std::function<void(Pitch)>& callback) {
    this->callback = callback;
}

const Pitch &PitchInputReader::getLastDetectedPitch() const {
    return lastDetectedPitch;
}

bool PitchInputReader::willExecuteCallBackOnInvalidPitches() const {
    return executeCallBackOnInvalidPitches;
}

void PitchInputReader::setExecuteCallBackOnInvalidPitches(bool executeCallBackOnInvalidPitches) {
    PitchInputReader::executeCallBackOnInvalidPitches = executeCallBackOnInvalidPitches;
}

PitchDetector* PitchInputReader::getPitchDetector() const {
    return pitchDetector.get();
}
