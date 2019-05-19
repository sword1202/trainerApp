//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <thread>
#include "VocalPartAudioPlayer.h"

int VocalPartAudioPlayer::readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) {
    return generator->readNextSamplesBatch((short*)intoBuffer, getVolume() <= 0.00001f);
}

void VocalPartAudioPlayer::prepareAndProvidePlaybackData(PlaybackData *playbackData) {
    playbackData->format = paInt16;
    playbackData->totalDurationInSeconds = generator->getDurationInSeconds();
    playbackData->framesPerBuffer = generator->getOutBufferSize();
    playbackData->sampleRate = generator->getSampleRate();
    playbackData->numChannels = 1;
}

VocalPartAudioPlayer::VocalPartAudioPlayer() {
    generator = new VocalPartAudioDataGenerator();
    setPlayerName("VxFileAudioPlayer");
}

int VocalPartAudioPlayer::getBufferSeek() const {
    return generator->getSeek();
}

void VocalPartAudioPlayer::setBufferSeek(int samplesCountSeek) {
    generator->setSeek(samplesCountSeek);
    AudioPlayer::setBufferSeek(samplesCountSeek);
}

VocalPartAudioPlayer::~VocalPartAudioPlayer() {
    delete generator;
}

void VocalPartAudioPlayer::onComplete() {
    AudioPlayer::onComplete();
}

bool VocalPartAudioPlayer::isPitchShiftingAvailable(int distance) const {
    return originalVocalPart.canBeShifted(distance);
}

void VocalPartAudioPlayer::setPitchShiftInSemiTones(int value) {
    if (value == getPitchShiftInSemiTones()) {
        return;
    }

    AudioPlayer::setPitchShiftInSemiTones(value);
    VocalPart vxFile = originalVocalPart.shifted(value);
    generator->setVocalPart(vxFile);
}

const VocalPart &VocalPartAudioPlayer::getVocalPart() const {
    return generator->getVocalPart();
}

void VocalPartAudioPlayer::setVocalPart(const VocalPart &vocalPart) {
    destroy();
    originalVocalPart = vocalPart;
    generator->resetVocalPart(vocalPart);
}

void VocalPartAudioPlayer::setTempoFactor(double tempoFactor) {
    if (getTempoFactor() == tempoFactor) {
        return;
    }

    AudioPlayer::setTempoFactor(tempoFactor);
    VocalPart vocalPart = originalVocalPart.withChangedTempo(tempoFactor);
    generator->setVocalPart(vocalPart);
}
