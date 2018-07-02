//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <thread>
#include "VxFileAudioPlayer.h"

static const int VX_FILE_GENERATOR_SLEEP_INTERVAL = 100;

int VxFileAudioPlayer::readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) {
    return generator->readNextSamplesBatch((short*)intoBuffer);
}

void VxFileAudioPlayer::prepareAndProvidePlaybackData(PlaybackData *playbackData) {
    generator->renderNextPitchIfPossible();
    playbackData->format = paInt16;
    playbackData->totalDurationInSeconds = generator->getDurationInSeconds();
    playbackData->framesPerBuffer = generator->getOutBufferSize();
    playbackData->sampleRate = generator->getSampleRate();
    playbackData->numChannels = 1;

    // pre-render data for first playback
    while (!generator->isPublished(0, generator->getOutBufferSize())) {
        assert(generator->renderNextPitchIfPossible());
    }

    generatorTask = new CppUtils::PeriodicallySleepingBackgroundTaskWithCallbacksQueue();
    generatorTask->runWithSleepingIntervalInMicroseconds([=]{
            while (generator->renderNextPitchIfPossible()) {
                generatorTask->processQueue();
                std::this_thread::sleep_for(std::chrono::microseconds(100000));
            }
        }, VX_FILE_GENERATOR_SLEEP_INTERVAL);
}

VxFileAudioPlayer::VxFileAudioPlayer(const VxFile &vxFile) {
    this->originalVxFile = vxFile;
    generator = new VxFileAudioDataGenerator(vxFile);
}

int VxFileAudioPlayer::getBufferSeek() const {
    return generator->getSeek();
}

void VxFileAudioPlayer::setBufferSeek(int samplesCountSeek) {
    generator->setSeek(samplesCountSeek);
    AudioPlayer::setBufferSeek(samplesCountSeek);
}

VxFileAudioPlayer::~VxFileAudioPlayer() {
    delete generator;
}

void VxFileAudioPlayer::onComplete() {
    AudioPlayer::onComplete();
}

void VxFileAudioPlayer::destroy(const std::function<void()>& onDestroyed) {
    generatorTask->stop([=] {
        delete this;
        if (onDestroyed) {
            onDestroyed();
        }
    });
}

bool VxFileAudioPlayer::isPitchShiftingAvailable(int distance) const {
    return originalVxFile.canBeShifted(distance);
}

void VxFileAudioPlayer::setPitchShiftInSemiTones(int value) {
    AudioPlayer::setPitchShiftInSemiTones(value);
    generatorTask->postCallback([=] {
        VxFile vxFile = originalVxFile.shifted(value);
        generator->setVxFile(vxFile);
    });
}

const VxFile &VxFileAudioPlayer::getVxFile() const {
    return generator->getVxFile();
}
