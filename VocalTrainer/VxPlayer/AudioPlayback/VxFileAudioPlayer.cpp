//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <thread>
#include "VxFileAudioPlayer.h"

int VxFileAudioPlayer::readNextSamplesBatch(void *intoBuffer, const PlaybackData &playbackData) {
    return generator->readNextSamplesBatch((short*)intoBuffer);
}

void VxFileAudioPlayer::prepareAndProvidePlaybackData(PlaybackData *playbackData) {
    generator->renderNextPitchIfPossible();
    playbackData->format = paInt16;
    playbackData->totalStreamSamplesCount = generator->getTotalSamplesCount();
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

VxFileAudioPlayer::VxFileAudioPlayer(VxFile *vxFile) : vxFile(vxFile) {
    generator = new VxFileAudioDataGenerator(vxFile);
}

void VxFileAudioPlayer::generatorThreadAction() {
    while (generatorThreadRunning) {
        while (generator->renderNextPitchIfPossible());
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    delete generator;
}

int VxFileAudioPlayer::getSamplesCountSeek() const {
    return generator->getSeek();
}

void VxFileAudioPlayer::setSamplesCountSeek(int samplesCountSeek) {
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
