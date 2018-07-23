//
// Created by Semyon Tikhonenko on 7/23/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "BaseWavAudioPlayer.h"
#include "AudioPlayer.h"
#include "WAVFile.h"

void BaseWavAudioPlayer::prepareAndProvidePlaybackData(AudioPlayer::PlaybackData *playbackData) {
    WavSetupData wavHeaderData = provideWavSetupData();
    if (wavHeaderData.size < WAVFile::DATA_POSITION) {
        throw std::runtime_error("Corrupted wav file");
    }

    WAVFile wavFile(wavHeaderData.wavHeaderData, WAVFile::DATA_POSITION);
    playbackData->sampleRate = wavFile.getSampleRate();
    if (wavFile.getAudioFormat() != WAVFile::PCM_AUDIO_FORMAT) {
        throw std::runtime_error("Unsupported wav audio format");
    }

    playbackData->numChannels = wavFile.getNumberOfChannels();
    int bytesPerChannel = wavFile.getBytesPerChannel();
    switch (bytesPerChannel) {
        case 1:
            playbackData->format = paInt8;
            break;
        case 2:
            playbackData->format = paInt16;
            break;
        case 4:
            playbackData->format = paInt32;
            break;
        default:
            throw std::runtime_error("Unsupported wav bytes per channel " + std::to_string(bytesPerChannel));
    }

    long pcmDataBytesCount = wavHeaderData.size - WAVFile::DATA_POSITION;

    if (pcmDataBytesCount % (bytesPerChannel * playbackData->numChannels) != 0) {
        throw std::runtime_error("Corrupted wav file");
    }

    playbackData->totalDurationInSeconds = calculateTotalDurationInSeconds(
            wavHeaderData.size, bytesPerChannel, *playbackData);
    playbackData->framesPerBuffer = 256;
}

BaseWavAudioPlayer::~BaseWavAudioPlayer() {

}

BaseWavAudioPlayer::BaseWavAudioPlayer() {
}

double BaseWavAudioPlayer::calculateTotalDurationInSeconds(int size, int bytesPerChannel, const AudioPlayer::PlaybackData &playbackData) {
    return samplesCountToSeconds(size - WAVFile::DATA_POSITION / (bytesPerChannel * playbackData.numChannels));
}

BaseWavAudioPlayer::WavSetupData::WavSetupData(const char *data, int size) : wavHeaderData(data), size(size) {
}
