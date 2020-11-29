//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <thread>
#include "VocalPartAudioPlayer.h"
#include "Algorithms.h"
#include "AudioUtils.h"
#include "MathUtils.h"
#include "Executors.h"

static constexpr int SAMPLE_RATE = 44100;

#define PITCHES_MAP_LOCK std::lock_guard<std::mutex> _(pitchesAudioDataMapMutex)
#define AUDIO_DATA_LOCK std::lock_guard<std::mutex> __(audioDataMutex)

constexpr int FADE_DURATION_IN_SAMPLES = 3000;

using namespace CppUtils;

void VocalPartAudioPlayer::providePlaybackData(PlaybackData *playbackData) {
    assert(sfz && "Call setSfz before calling prepare");

    playbackData->format = paInt16;
    playbackData->totalDurationInSeconds = vocalPart.getDurationInSeconds();
    playbackData->framesPerBuffer = 256;
    playbackData->sampleRate = SAMPLE_RATE;
    playbackData->numChannels = 1;
    updateAudioData();
}

size_t VocalPartAudioPlayer::getAudioDataBufferSize() const {
    return static_cast<size_t>(
            Math::RoundToInt(vocalPart.getDurationInSeconds() * getPlaybackData().sampleRate *
                    getPlaybackData().numChannels));
}

VocalPartAudioPlayer::VocalPartAudioPlayer() {
    setPlayerName("VxFileAudioPlayer");
}

VocalPartAudioPlayer::~VocalPartAudioPlayer() {
    for (const auto& pair : pitchesAudioDataMap) {
        delete pair.second;
    }

    AUDIO_DATA_LOCK;
    while (!audioDataPool.empty()) {
        delete audioDataPool.front();
        audioDataPool.pop();
    }
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
    updateAudioDataOnBackground();
}

void VocalPartAudioPlayer::onTonalityChanged(int value) {
    vocalPart = originalVocalPart.shifted(value);
    updateAudioDataOnBackground();
}

void VocalPartAudioPlayer::setSfz(const SfzFile* sfz) {
    this->sfz = sfz;
}

void VocalPartAudioPlayer::updateAudioData() {
    std::lock_guard<std::mutex> _(updateAudioDataMutex);

    for (const NoteInterval& note : vocalPart.getPitches()) {
        int index = note.pitch.getPerfectFrequencyIndex();
        bool exists;
        {
            PITCHES_MAP_LOCK;
            exists = static_cast<bool>(pitchesAudioDataMap.count(index));
        }
        if (!exists) {
            const SfzRegion& region = sfz->findRegion(note.pitch);
            int diff = region.keyCenter - index;
            if (diff == 0) {
                PITCHES_MAP_LOCK;
                pitchesAudioDataMap[index] = new AudioData(
                        region.audioData.data() + WAVFile::DATA_POSITION,
                        region.audioData.size() - WAVFile::DATA_POSITION);
            } else {
                soundTouch.setPitchSemiTones(diff);
                tempFloatSamplesBuffer.resize(region.audioData.size() / 2);
                auto config = WAVFile::getFloatSamples(region.audioData.data(),
                        static_cast<int>(region.audioData.size()),
                        tempFloatSamplesBuffer.data());
                uint samplesNumber = static_cast<uint>(tempFloatSamplesBuffer.size() / config.numberOfChannels);
                soundTouch.putSamples(tempFloatSamplesBuffer.data(), samplesNumber);
                soundTouch.receiveSamples(tempFloatSamplesBuffer.data(), samplesNumber);
                AudioData* noteAudioData = new AudioData(region.audioData.size(), 0);
                AudioUtils::FloatSamplesIntoInt16Samples(tempFloatSamplesBuffer.data(),
                        static_cast<int>(tempFloatSamplesBuffer.size()),
                        reinterpret_cast<short *>(noteAudioData->data()));
                {
                    PITCHES_MAP_LOCK;
                    pitchesAudioDataMap[index] = noteAudioData;
                }
            }
        }
    }

    size_t bufferSize = getAudioDataBufferSize();
    std::vector<short>* result = new std::vector<short>(bufferSize, 0);

    for (const NoteInterval& note : vocalPart.getPitches()) {
        double pitchStart = vocalPart.ticksToSeconds(note.startTickNumber);
        double pitchDuration = vocalPart.ticksToSeconds(note.ticksCount);

        int pitchStartDataIndex = secondsSeekToBufferSeek(pitchStart);
        int pitchBufferLength = secondsSeekToBufferSeek(pitchDuration);

        const AudioData *pitchAudioData;
        {
            PITCHES_MAP_LOCK;
            pitchAudioData = pitchesAudioDataMap[note.pitch.getPerfectFrequencyIndex()];
        }

        const auto& region = sfz->findRegion(note.pitch);
        short* resultPtr = result->data() + pitchStartDataIndex;
        if (pitchAudioData->size() > pitchBufferLength * 2) {
            tempPitchBuffer.resize(static_cast<size_t>(pitchBufferLength));
            const short* asShort = reinterpret_cast<const short *>(pitchAudioData->data());
            std::copy(asShort, asShort + pitchBufferLength, tempPitchBuffer.data());

            int fadeDuration = FADE_DURATION_IN_SAMPLES * getPlaybackData().numChannels;
            AudioUtils::MakeLinearFadeOutAtEnding(tempPitchBuffer.data(),
                    static_cast<int>(tempPitchBuffer.size()),
                    fadeDuration
                    );
            AudioUtils::Mix2Sounds(tempPitchBuffer.data(),
                    resultPtr,
                    static_cast<int>(tempPitchBuffer.size()),
                    resultPtr);
        } else {
            AudioUtils::Mix2Sounds(reinterpret_cast<const short *>(pitchAudioData->data()),
                    resultPtr,
                    static_cast<int>(pitchAudioData->size() / 2),
                    resultPtr);
        }
    }

    AUDIO_DATA_LOCK;
    audioDataPool.push(result);
}

int VocalPartAudioPlayer::getAudioDataSizeInBytes() {
    return static_cast<int>(getAudioDataBufferSize() * sizeof(short));
}

const char *VocalPartAudioPlayer::provideAudioBuffer() {
    AUDIO_DATA_LOCK;
    return reinterpret_cast<const char *>(audioDataPool.back()->data());
}

int VocalPartAudioPlayer::readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) {
    int result = BaseRawPcmAudioDataPlayer::readNextSamplesBatch(intoBuffer, framesCount, playbackData);
    AUDIO_DATA_LOCK;
    while (audioDataPool.size() >= 2) {
        delete audioDataPool.front();
        audioDataPool.pop();
    }
    return result;
}

void VocalPartAudioPlayer::updateAudioDataOnBackground() {
    Executors::ExecuteOnBackgroundThread([=] {
        this->updateAudioData();
    });
}
