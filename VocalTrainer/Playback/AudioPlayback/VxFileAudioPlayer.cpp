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
    playbackData->format = paInt16;
    playbackData->totalDurationInSeconds = generator->getDurationInSeconds();
    playbackData->framesPerBuffer = generator->getOutBufferSize();
    playbackData->sampleRate = generator->getSampleRate();
    playbackData->numChannels = 1;
}

VxFileAudioPlayer::VxFileAudioPlayer() {
    generator = new VxFileAudioDataGenerator();
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

bool VxFileAudioPlayer::isPitchShiftingAvailable(int distance) const {
    return originalVxFile.canBeShifted(distance);
}

void VxFileAudioPlayer::setPitchShiftInSemiTones(int value) {
    if (value == getPitchShiftInSemiTones()) {
        return;
    }

    AudioPlayer::setPitchShiftInSemiTones(value);
    VxFile vxFile = originalVxFile.shifted(value);
    generator->setVxFile(vxFile);
}

const VxFile &VxFileAudioPlayer::getVxFile() const {
    return generator->getVxFile();
}

void VxFileAudioPlayer::setVxFile(const VxFile& vxFile) {
    destroy();
    originalVxFile = vxFile;
    generator->resetVxFile(vxFile);
}

void VxFileAudioPlayer::setTempoFactor(double tempoFactor) {
    if (getTempoFactor() == tempoFactor) {
        return;
    }

    AudioPlayer::setTempoFactor(tempoFactor);
    VxFile vxFile = originalVxFile.withChangedTempo(tempoFactor);
    generator->setVxFile(vxFile);
}
