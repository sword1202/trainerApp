//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "WavAudioPlayer.h"
#include <boost/iostreams/stream.hpp>

int WavAudioPlayer::readNextSamplesBatch(void *intoBuffer, int framesCount,
        const AudioPlayer::PlaybackData &playbackData) {
    int frameSize = getSampleSize();
    int seek = getBufferSeek();
    int offset = WAVFile::DATA_POSITION + seek * frameSize;
    int size = std::min(framesCount * frameSize, (int)audioData.size() - offset);
    if (size > 0) {
        memcpy(intoBuffer, audioData.data() + offset, size);
        int newSeek = size / frameSize + seek;
        setBufferSeek(newSeek);
    }
    
    return size / frameSize;
}

void WavAudioPlayer::prepareAndProvidePlaybackData(AudioPlayer::PlaybackData *playbackData) {
    auto* stream =  new boost::iostreams::stream<boost::iostreams::array_source>(audioData.data(), audioData.size());
    WAVFile wavFile(stream);
    wavFile.setDeleteStreamOnDestructor(true);
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

    long pcmDataBytesCount = audioData.size() - WAVFile::DATA_POSITION;
    if (pcmDataBytesCount < 0) {
        throw std::runtime_error("Corrupted wav file");
    }

    if (pcmDataBytesCount % (bytesPerChannel * playbackData->numChannels) != 0) {
        throw std::runtime_error("Corrupted wav file");
    }

    playbackData->totalDurationInSeconds = samplesCountToSeconds(pcmDataBytesCount / (bytesPerChannel * playbackData->numChannels));
    playbackData->framesPerBuffer = 256;
}

WavAudioPlayer::~WavAudioPlayer() {

}

int WavAudioPlayer::getBufferSeek() const {
    return bufferSeek;
}

void WavAudioPlayer::setBufferSeek(int bufferSeek) {
    AudioPlayer::setBufferSeek(bufferSeek);
    this->bufferSeek = bufferSeek;
}

void WavAudioPlayer::setAudioData(std::string &&audioData) {
    this->audioData = std::move(audioData);
}

WavAudioPlayer::WavAudioPlayer() {
    bufferSeek = 0;
}
