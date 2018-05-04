//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <thread>
#include "VxFileAudioPlayer.h"

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
    startGeneratorThread();
}

void VxFileAudioPlayer::startGeneratorThread() {
    assert(!generatorThreadRunning);
    generatorThreadRunning = true;
    std::thread thread([this] {
        generatorThreadAction();
    });
    thread.detach();
}

VxFileAudioPlayer::VxFileAudioPlayer(const VxFile *vxFile) : vxFile(vxFile) {
    generatorThreadRunning = false;
    generator = new VxFileAudioDataGenerator(vxFile);
}

void VxFileAudioPlayer::generatorThreadAction() {
    while (generatorThreadRunning) {
        while (generator->renderNextPitchIfPossible());
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    delete generator;
}

int VxFileAudioPlayer::getBufferSeek() const {
    return generator->getSeek();
}

void VxFileAudioPlayer::setBufferSeek(int samplesCountSeek) {
    generator->setSeek(samplesCountSeek);
}

VxFileAudioPlayer::~VxFileAudioPlayer() {
    stopGeneratorThread();
}

void VxFileAudioPlayer::onComplete() {
    AudioPlayer::onComplete();
}

void VxFileAudioPlayer::stopGeneratorThread() {
    generatorThreadRunning = false;
}
