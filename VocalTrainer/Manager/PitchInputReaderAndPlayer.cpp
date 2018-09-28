//
// Created by Semyon Tikhonenko on 7/4/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "PitchInputReaderAndPlayer.h"
#include "AubioPitchDetector.h"
#include "Executors.h"

static constexpr float THRESHOLD = 0.1;
static const int BUFFER_SIZE = 1024;
static const int SMOOTH_LEVEL = 4;

PitchInputReaderAndPlayer::PitchInputReaderAndPlayer() {
    AubioPitchDetector* pitchDetector = new AubioPitchDetector();
    pitchDetector->setThreshold(THRESHOLD);

    audioInputReader = CreateDefaultAudioInputReader(BUFFER_SIZE);
    audioInputReader->setCallback(audioInputCallbacks);

    AudioPlayer::PlaybackData playbackData;
    playbackData.framesPerBuffer = BUFFER_SIZE;
    playbackData.format = paInt16;
    playbackData.numChannels = 1;
    playbackData.sampleRate = audioInputReader->getSampleRate();
    audioInputPlayer = new RealtimeStreamingAudioPlayer(playbackData);
    audioInputPlayer->prepare();
    audioInputCallbacks.push_back([=] (const int16_t* buffer, int size) {
        audioInputPlayer->pushAudioData(buffer, size * sizeof(int16_t));
    });

    init(audioInputReader, SMOOTH_LEVEL, pitchDetector, true);
}

void PitchInputReaderAndPlayer::pitchDetected(float frequency, double time) {
    CppUtils::Executors::ExecuteOnMainThread([=] {
        PitchInputReaderCollector::pitchDetected(frequency, time);
    });
}

void PitchInputReaderAndPlayer::setAudioInputReaderCallback(
        AudioInputReader *audioInputReader,
        const AudioInputReader::Callback &callback) {
    audioInputCallbacks.push_back(callback);
}

PitchInputReaderAndPlayer::~PitchInputReaderAndPlayer() {
    delete audioInputPlayer;
}

void PitchInputReaderAndPlayer::setInputVolume(float value) {
    audioInputReader->setInputVolume(value);
}

float PitchInputReaderAndPlayer::getInputVolume() const {
    return audioInputReader->getInputVolume();
}

void PitchInputReaderAndPlayer::setOutputVolume(float value) {
    audioInputPlayer->setVolume(value);
}

float PitchInputReaderAndPlayer::getOutputVolume() const {
    return audioInputPlayer->getVolume();
}
