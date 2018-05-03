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
    int size = wavFile->readData(intoBuffer, framesCount * frameSize, seek * frameSize);
    if (size > 0) {
        int newSeek = size / frameSize + seek;
        setBufferSeekIfNotModified(newSeek, seek);
    }
    
    return size / frameSize;
}

void WavAudioPlayer::prepareAndProvidePlaybackData(AudioPlayer::PlaybackData *playbackData) {
    auto* stream =  new boost::iostreams::stream<boost::iostreams::array_source>(audioData.data(), audioData.size());
    wavFile = new WAVFile(stream);
    wavFile->setDeleteStreamOnDestructor(true);
    playbackData->sampleRate = wavFile->getSampleRate();
    if (wavFile->getAudioFormat() != WAVFile::PCM_AUDIO_FORMAT) {
        throw std::runtime_error("Unsupported wav audio format");
    }
    
    playbackData->numChannels = wavFile->getNumberOfChannels();
    int bytesPerChannel = wavFile->getBytesPerChannel();
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
    playbackData->framesPerBuffer = paFramesPerBufferUnspecified;
}

WavAudioPlayer::WavAudioPlayer(std::string &&audioData) : AudioFilePlayer(std::move(audioData)) {

}

WavAudioPlayer::~WavAudioPlayer() {
    if (wavFile) {
        delete wavFile;
    }
}
