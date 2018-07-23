//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "WavAudioPlayer.h"
#include "WAVFile.h"

int WavAudioPlayer::readNextSamplesBatch(void *intoBuffer, int framesCount,
        const AudioPlayer::PlaybackData &playbackData) {
    int frameSize = getSampleSize();
    int seek = getBufferSeek();
    int offset = WAVFile::DATA_POSITION + seek * frameSize;
    int size = std::min(framesCount * frameSize, (int)audioData.size() - offset);
    if (size > 0) {
        memcpy(intoBuffer, audioData.data() + offset, size);
        moveBufferSeekIfNotChangedBefore(size / frameSize, seek);
    }
    
    return size / frameSize;
}

void WavAudioPlayer::setAudioData(std::string &&audioData) {
    this->audioData = std::move(audioData);
}

WavAudioPlayer::WavSetupData WavAudioPlayer::provideWavSetupData() {
    return WavSetupData(audioData.data(), audioData.size());
}
