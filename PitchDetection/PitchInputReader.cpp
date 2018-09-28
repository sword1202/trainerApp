//
// Created by Semyon Tikhonenko on 8/7/16.
//

#include <cstdlib>
#include <iostream>
#include "PitchInputReader.h"

using namespace CppUtils;

PitchInputReader::PitchInputReader(AudioInputReader *audioInputReader, PitchDetector* pitchDetector, int smoothLevel) :
        audioInputReader(audioInputReader), pitchDetector(pitchDetector),
        smoothingAudioBuffer((size_t) smoothLevel, (size_t) audioInputReader->getMaximumBufferSize()) {
    pitchDetector->init(audioInputReader->getMaximumBufferSize() * smoothLevel, audioInputReader->getSampleRate());
    audioInputReader->callbacks.push_back([=] (const int16_t* buffer, int size) {
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
    });
}

bool PitchInputReader::willDestroyAudioInputReaderOnDestructor() const {
    return destroyAudioInputReaderOnDestructor;
}

void PitchInputReader::setDestroyAudioInputReaderOnDestructor(bool destroyAudioInputReaderOnDestructor) {
    PitchInputReader::destroyAudioInputReaderOnDestructor = destroyAudioInputReaderOnDestructor;
}

PitchInputReader::~PitchInputReader() {
    if (destroyAudioInputReaderOnDestructor) {
        delete audioInputReader;
    }
}

void PitchInputReader::setCallback(const std::function<void(Pitch)>& callback) {
    this->callback = callback;
}

void PitchInputReader::start() {
//    running = true;
    audioInputReader->start();
}

void PitchInputReader::stop() {
    audioInputReader->stop();
//    running = false;
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

bool PitchInputReader::isRunning() const {
    return true;
}

PitchDetector* PitchInputReader::getPitchDetector() const {
    return pitchDetector.get();
}
