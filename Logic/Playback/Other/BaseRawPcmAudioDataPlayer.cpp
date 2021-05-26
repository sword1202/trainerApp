//
// Created by Semyon Tikhonenko on 10/5/18.
//

#include "BaseRawPcmAudioDataPlayer.h"

int BaseRawPcmAudioDataPlayer::readNextSamplesBatch(void *intoBuffer, int framesCount,
                                         const PlaybackData &playbackData) {
    int frameSize = getSampleSize();
    int seek = getBufferSeek();
    int offset = seek * frameSize;

    AudioDataBufferConstPtr audioData = provideAudioBuffer();
    int readCount = audioData->read(intoBuffer, offset, framesCount * frameSize);
    int readFramesCount = readCount / frameSize;
    moveBufferSeekIfNotChangedBefore(readFramesCount, seek);
    return readFramesCount;
}

void BaseRawPcmAudioDataPlayer::providePlaybackData(PlaybackData *playbackData) {
    playbackData->totalDurationInSeconds = bufferSeekToSecondsSeek(getAudioDataSizeInBytes() / getSampleSize());
}

void BaseRawPcmAudioDataPlayer::setPlaybackData(const WavConfig &wavConfig, int framesPerBuffer) {
    BaseAudioPlayer::setPlaybackData(PlaybackData(wavConfig, framesPerBuffer));
}
