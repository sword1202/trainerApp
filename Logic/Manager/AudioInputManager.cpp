//
// Created by Semyon Tikhonenko on 7/4/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "AudioInputManager.h"
#include "AubioPitchDetector.h"
#include "Executors.h"
#include "PortAudioInputReader.h"
#include "AudioAverageInputLevelMonitor.h"
#include "AudioUtils.h"

static constexpr float THRESHOLD = 0.1;
static const int BUFFER_SIZE = 1024;
static const int SMOOTH_LEVEL = 4;

using namespace CppUtils;

AudioInputManager::AudioInputManager(const char* deviceName) {
    AubioPitchDetector* pitchDetector = new AubioPitchDetector();
    pitchDetector->setThreshold(THRESHOLD);
    
    audioInputReader = new PortAudioInputReader(BUFFER_SIZE, true, deviceName);
    pitchesRecorder = new AudioInputPitchesRecorder();
    pitchesRecorder->init(audioInputReader, SMOOTH_LEVEL, pitchDetector);
    audioInputReader->start();

    audioRecorder = new AudioInputRecorder();
}

AudioInputManager::~AudioInputManager() {
    delete audioInputReader;
    delete audioRecorder;
    delete pitchesRecorder;
}

void AudioInputManager::setInputVolume(float value) {
    audioInputReader->setInputVolume(value);
}

float AudioInputManager::getInputVolume() const {
    return audioInputReader->getInputVolume();
}

void AudioInputManager::setOutputVolume(float value) {
    audioInputReader->setOutputVolume(value);
}

float AudioInputManager::getOutputVolume() const {
    return audioInputReader->getOutputVolume();
}

const char* AudioInputManager::getInputDeviceName() const {
    return audioInputReader->getDeviceName();
}

void AudioInputManager::setInputDeviceName(const char *deviceName) const {
    audioInputReader->setDeviceName(deviceName);
}

void AudioInputManager::addAudioInputReaderCallback(const AudioInputReader::Callback &callback) {
    audioInputReader->callbacks.addListener(callback);
}

void AudioInputManager::startPitchDetection(double seek) {
    setAudioRecorderSeek(seek);
    setPitchesRecorderSeek(seek);
    audioInputReader->callbacks.addListener(pitchesRecorder);
    if (audioRecordingEnabled) {
        audioInputReader->callbacks.addListener(audioRecorder);
    }
}

void AudioInputManager::stopPitchDetection() {
    audioInputReader->callbacks.removeListeners(pitchesRecorder, audioRecorder);
}

void AudioInputManager::addAudioInputLevelMonitor(const std::function<void(double)> &callback) {
    addAudioInputReaderCallback(AudioAverageInputLevelMonitor(BUFFER_SIZE, [=] (double value) {
        Executors::ExecuteOnMainThread([=] {
            callback(value);
        });
    }));
}

bool AudioInputManager::isAudioRecordingEnabled() const {
    return audioRecordingEnabled;
}

void AudioInputManager::setAudioRecordingEnabled(bool audioDataCollectorEnabled) {
    this->audioRecordingEnabled = audioDataCollectorEnabled;
}

void AudioInputManager::setAudioRecorderSeek(double timeSeek) {
    int seek = AudioUtils::GetSamplesByteIndexFromTime(timeSeek, audioInputReader->getSampleRate(),
            audioInputReader->getSampleSizeInBytes());
    audioRecorder->setSeek(seek);
}

void AudioInputManager::setPitchesRecorderSeek(double timeSeek) {
    pitchesRecorder->setSeek(timeSeek);
}

CppUtils::ListenersSet<const Pitch &, double> &AudioInputManager::getPitchDetectedListeners() {
    return pitchesRecorder->pitchDetectedListeners;
}

const PitchesCollection *AudioInputManager::getRecordedPitches() const {
    return pitchesRecorder->getPitches();
}

std::string AudioInputManager::getRecordedDataInWavFormat() const {
    WavConfig wavConfig = audioInputReader->generateWavConfig();
    const std::string& recordedData = audioRecorder->getRecordedData();
    return WAVFile::addWavHeaderToRawPcmData<std::string>(recordedData.data(), recordedData.size(), wavConfig);
}
