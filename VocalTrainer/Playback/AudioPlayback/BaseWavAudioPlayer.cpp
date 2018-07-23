//
// Created by Semyon Tikhonenko on 7/23/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "BaseWavAudioPlayer.h"
#include "AudioPlayer.h"
#include "WAVFile.h"

void BaseWavAudioPlayer::prepareAndProvidePlaybackData(AudioPlayer::PlaybackData *playbackData) {
    WavSetupData wavHeaderData = provideWavSetupData();
    if (wavHeaderData.totalSize < WAVFile::DATA_POSITION) {
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

    long pcmDataBytesCount = wavHeaderData.totalSize - WAVFile::DATA_POSITION;

    if (pcmDataBytesCount % (bytesPerChannel * playbackData->numChannels) != 0) {
        throw std::runtime_error("Corrupted wav file");
    }

    playbackData->totalDurationInSeconds = samplesCountToSeconds(pcmDataBytesCount / (bytesPerChannel * playbackData->numChannels));
    playbackData->framesPerBuffer = 256;
}

BaseWavAudioPlayer::~BaseWavAudioPlayer() {

}

int BaseWavAudioPlayer::getBufferSeek() const {
    return bufferSeek;
}

void BaseWavAudioPlayer::setBufferSeek(int bufferSeek) {
    AudioPlayer::setBufferSeek(bufferSeek);
    this->bufferSeek = bufferSeek;
}

BaseWavAudioPlayer::BaseWavAudioPlayer() {
    bufferSeek = 0;
}

BaseWavAudioPlayer::WavSetupData::WavSetupData(const char *data, int totalSize) : wavHeaderData(data), totalSize(totalSize) {
}
