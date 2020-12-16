//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <thread>
#include "VocalPartAudioPlayer.h"
#include "SfzPitchRenderer.h"

constexpr int SAMPLES_PER_BUFFER = 256;
constexpr int NUMBER_OF_CHANNELS = 2;

int VocalPartAudioPlayer::readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) {
    bool moveSeekAndFillWithZero = getVolume() <= 0.00001f;
    return generator->readNextSamplesBatch((short*)intoBuffer, moveSeekAndFillWithZero);
}

void VocalPartAudioPlayer::providePlaybackData(PlaybackData *playbackData) {
    playbackData->samplesPerBuffer = SAMPLES_PER_BUFFER;
    playbackData->numberOfChannels = NUMBER_OF_CHANNELS;
    playbackData->sampleRate = 44100;
    playbackData->numberOfChannels = NUMBER_OF_CHANNELS;

    generator->init(*playbackData);
    generator->resetVocalPart(originalVocalPart);
    playbackData->totalDurationInSeconds = generator->getDurationInSeconds();
}

VocalPartAudioPlayer::VocalPartAudioPlayer() {
    setPlayerName("VocalPartAudioPlayer");
    generator = new VocalPartAudioDataGenerator(new SfzPitchRenderer());
}

int VocalPartAudioPlayer::getBufferSeek() const {
    return generator->getSeek();
}

void VocalPartAudioPlayer::setBufferSeek(int samplesCountSeek) {
    generator->setSeek(samplesCountSeek);
    BaseAudioPlayer::setBufferSeek(samplesCountSeek);
}

VocalPartAudioPlayer::~VocalPartAudioPlayer() {
    delete generator;
}

void VocalPartAudioPlayer::onComplete() {
    BaseAudioPlayer::onComplete();
}

bool VocalPartAudioPlayer::isPitchShiftingAvailable(int distance) const {
    return originalVocalPart.canBeShifted(distance);
}

const VocalPart &VocalPartAudioPlayer::getVocalPart() const {
    return generator->getVocalPart();
}

void VocalPartAudioPlayer::setVocalPart(const VocalPart &vocalPart) {
    destroy();
    originalVocalPart = vocalPart;
    if (generator) {
        generator->resetVocalPart(vocalPart);
    }
}

void VocalPartAudioPlayer::setTempoFactor(double tempoFactor) {
    if (getTempoFactor() == tempoFactor) {
        return;
    }

    BaseAudioPlayer::setTempoFactor(tempoFactor);
    VocalPart vocalPart = originalVocalPart.withChangedTempo(tempoFactor);
    generator->setVocalPart(vocalPart);
}

void VocalPartAudioPlayer::onTonalityChanged(int value) {
    VocalPart vxFile = originalVocalPart.shifted(value);
    generator->setVocalPart(vxFile);
}

void VocalPartAudioPlayer::destroy() {
    BaseAudioPlayer::destroy();
}
