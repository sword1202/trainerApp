//
// Created by Semyon Tikhonenko on 10/5/18.
//

#include "BaseRawPcmAudioDataPlayer.h"
#include <cstring>

int BaseRawPcmAudioDataPlayer::readNextSamplesBatch(void *intoBuffer, int framesCount,
                                         const PlaybackData &playbackData) {
    int frameSize = getSampleSize();
    int seek = getBufferSeek();
    int offset = seek * frameSize;

    int audioDataSize = getAudioDataSizeInBytes();
    const char* audioData = provideAudioBuffer(offset);
    int size = std::min(framesCount * frameSize, audioDataSize - offset);
    if (size > 0) {
        memcpy(intoBuffer, audioData, (size_t)size);
        moveBufferSeekIfNotChangedBefore(size / frameSize, seek);
    }

    return size / frameSize;
}

void BaseRawPcmAudioDataPlayer::providePlaybackData(PlaybackData *playbackData) {
    playbackData->totalDurationInSeconds = bufferSeekToSecondsSeek(getAudioDataSizeInBytes() / getSampleSize());
}

void BaseRawPcmAudioDataPlayer::setPlaybackData(const WavConfig &wavConfig, int framesPerBuffer) {
    BaseAudioPlayer::setPlaybackData(PlaybackData(wavConfig, framesPerBuffer));
}
