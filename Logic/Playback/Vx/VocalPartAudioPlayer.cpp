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
    assert(generator && "prepare should be called after setVocalPart");
    playbackData->samplesPerBuffer = SAMPLES_PER_BUFFER;
    playbackData->numberOfChannels = NUMBER_OF_CHANNELS;
    playbackData->sampleRate = 44100;
    playbackData->numberOfChannels = NUMBER_OF_CHANNELS;

    generator->init(*playbackData);
    generator->setVocalPart(originalVocalPart);
    playbackData->totalDurationInSeconds = generator->getDurationInSeconds();
}

VocalPartAudioPlayer::VocalPartAudioPlayer() {
    setPlayerName("VocalPartAudioPlayer");
}

int VocalPartAudioPlayer::getBufferSeek() const {
    return generator->getSeek();
}

void VocalPartAudioPlayer::setBufferSeek(int samplesCountSeek) {
    generator->setSeek(samplesCountSeek);
    BaseAudioPlayer::setBufferSeek(samplesCountSeek);
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
    reset();
    originalVocalPart = vocalPart;
    generator = new VocalPartAudioDataGenerator(new SfzPitchRenderer());
    generator->setSeek(0);
    generator->setVocalPart(vocalPart);
}

void VocalPartAudioPlayer::onTonalityChanged(int value) {
    VocalPart vxFile = originalVocalPart.shifted(value);
    generator->setVocalPart(vxFile);
}

void VocalPartAudioPlayer::reset() {
    BaseAudioPlayer::reset();
    delete generator;
    generator = nullptr;
}

void VocalPartAudioPlayer::onTempoFactorChanged(double value, double oldValue) {
    VocalPart vocalPart = originalVocalPart.withChangedTempo(value);
    generator->setVocalPart(std::move(vocalPart));
}
