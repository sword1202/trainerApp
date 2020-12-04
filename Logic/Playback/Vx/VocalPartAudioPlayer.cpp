//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <thread>
#include "VocalPartAudioPlayer.h"
#include "Algorithms.h"
#include "AudioUtils.h"
#include "MathUtils.h"

constexpr int FADE_DURATION_IN_SAMPLES = 3000;
constexpr int MAX_SIMULTANEOUSLY_PLAYING_PITCHES = 3;

using namespace CppUtils;

#define PLAYING_PITCHES_LOCK std::lock_guard<std::mutex> _(playingPitchesMutex)

VocalPartAudioPlayer::VocalPartAudioPlayer() {
    setPlayerName("VocalPartAudioPlayer");
    soundsToMix.reserve(10);
    soundsToMixSizes.reserve(10);
}

int VocalPartAudioPlayer::readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) {
    int bufferSeek = getBufferSeek();
    double seek = bufferSeekToSecondsSeek(bufferSeek);
    double batchTime = framesCount / double(playbackData.sampleRate);
    int maxSoundBufferSize = framesCount * playbackData.numChannels;
    soundsToMix.clear();
    soundsToMixSizes.clear();
    vocalPart.iteratePitchesIndexesInTimeRange(seek, seek + batchTime, [=] (int index) {
        PlayingPitch* playingPitch = this->findPlayingPitch(index);
        const Pitch& pitch = vocalPart.getPitches()[index].pitch;
        if (!playingPitch) {
            PlayingPitch newPitch;
            newPitch.vocalPartPitchIndex = index;
            newPitch.region = &sfz->findRegion(pitch);
            {
                PLAYING_PITCHES_LOCK;
                playingPitches.push_back(newPitch);
                playingPitch = &playingPitches.back();
            }
        }
        
        int rawPcmBufferSize = playingPitch->region->getRawPcmBytesCount() / sizeof(short);
        int rawPcmBufferOffset = playingPitch->bufferSeek * playbackData.numChannels;
        rawPcmBufferSize -= rawPcmBufferOffset;
        int soundBufferSize = std::min(rawPcmBufferSize, maxSoundBufferSize);
        const short* soundBegin = reinterpret_cast<const short *>(playingPitch->region->getRawPcmPtr()) + rawPcmBufferOffset;
        int pitchShift = pitch.getMidiIndex() - playingPitch->region->keyCenter;
        if (pitchShift != 0) {
            AudioUtils::Int16SamplesIntoFloatSamples(soundBegin, soundBufferSize, tempFloatBuffer.data());
            soundTouch.setPitchSemiTones(pitchShift);
            uint soundTouchSamplesCount = static_cast<uint>(soundBufferSize / playbackData.numChannels);
            soundTouch.putSamples(tempFloatBuffer.data(), soundTouchSamplesCount);
            soundTouch.receiveSamples(tempFloatBuffer.data(), soundTouchSamplesCount);
            std::vector<short>* tempShortBuffer = tempShortBufferPool.getObject();
            AudioUtils::FloatSamplesIntoInt16Samples(tempFloatBuffer.data(), soundBufferSize, tempShortBuffer->data());
            soundBegin = tempShortBuffer->data();
        }

        soundsToMix.push_back(soundBegin);
        soundsToMixSizes.push_back(soundBufferSize);
    });

    AudioUtils::MixSounds(soundsToMix, soundsToMixSizes, static_cast<short *>(intoBuffer));
    tempShortBufferPool.returnAll();

    int moveSeekBy = std::min(framesCount, secondsSeekToBufferSeek(getTrackDurationInSeconds()) - bufferSeek);
    moveBufferSeekIfNotChangedBefore(moveSeekBy, bufferSeek);

    return moveSeekBy;
}

void VocalPartAudioPlayer::providePlaybackData(PlaybackData *playbackData) {
    assert(sfz && "setSfz should be called before prepare");
    assert(originalVocalPart.getDurationInSeconds() > 0 && "setVocalPart should be called before prepare");
    *playbackData = PlaybackData(sfz->getWavConfig(), 256 /*frames per buffer*/);
    playbackData->totalDurationInSeconds = vocalPart.getDurationInSeconds();

    soundTouch.setChannels(static_cast<uint>(playbackData->numChannels));
    soundTouch.setSampleRate(static_cast<uint>(playbackData->sampleRate));

    tempFloatBuffer.resize(static_cast<size_t>(playbackData->framesPerBuffer) * playbackData->numChannels);
    tempShortBufferPool.init(MAX_SIMULTANEOUSLY_PLAYING_PITCHES, std::vector<short>(tempFloatBuffer.size(), 0));
}

VocalPartAudioPlayer::PlayingPitch *VocalPartAudioPlayer::findPlayingPitch(int vocalPartPitchIndex) {
    PLAYING_PITCHES_LOCK;
    auto iter = std::find_if(playingPitches.begin(), playingPitches.end(), [=] (const PlayingPitch& value) {
        return value.vocalPartPitchIndex == vocalPartPitchIndex;
    });

    if (iter == playingPitches.end()) {
        return nullptr;
    } else {
        return &*iter;
    }
}

VocalPartAudioPlayer::~VocalPartAudioPlayer() {
}

bool VocalPartAudioPlayer::isPitchShiftingAvailable(int distance) const {
    return originalVocalPart.canBeShifted(distance);
}

const VocalPart &VocalPartAudioPlayer::getVocalPart() const {
    return vocalPart;
}

void VocalPartAudioPlayer::setVocalPart(const VocalPart &vocalPart) {
    assert(sfz && "call setSfz before calling setVocalPart");
    destroy();
    originalVocalPart = vocalPart;
    this->vocalPart = vocalPart;
}

void VocalPartAudioPlayer::setTempoFactor(double tempoFactor) {
    if (getTempoFactor() == tempoFactor) {
        return;
    }

    PortAudioPlayer::setTempoFactor(tempoFactor);
    vocalPart = originalVocalPart.withChangedTempo(tempoFactor);
}

void VocalPartAudioPlayer::onTonalityChanged(int value) {
    vocalPart = originalVocalPart.shifted(value);
}

void VocalPartAudioPlayer::setSfz(const SfzFile* sfz) {
    this->sfz = sfz;
}

void VocalPartAudioPlayer::onBufferSeekChanged(int seekBefore, int seekNow) {
    int diff = seekNow - seekBefore;
    PLAYING_PITCHES_LOCK;
    std::remove_if(playingPitches.begin(), playingPitches.end(), [=] (PlayingPitch& pitch) {
        pitch.bufferSeek += diff;
        return pitch.bufferSeek * getSampleSize() >= pitch.region->getRawPcmBytesCount();
    });
}
