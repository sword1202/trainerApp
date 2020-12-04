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
    const char* audioData = provideAudioBuffer();
    int size = std::min(framesCount * frameSize, audioDataSize - offset);
    if (size > 0) {
        memcpy(intoBuffer, audioData + offset, (size_t)size);
        moveBufferSeekIfNotChangedBefore(size / frameSize, seek);
    }

    return size / frameSize;
}

void BaseRawPcmAudioDataPlayer::providePlaybackData(PlaybackData *playbackData) {
    playbackData->totalDurationInSeconds = samplesCountToSeconds(getAudioDataSizeInBytes() / getSampleSize());
}

void BaseRawPcmAudioDataPlayer::setPlaybackData(const WavConfig &wavConfig, int framesPerBuffer) {
    PortAudioPlayer::setPlaybackData(PlaybackData(wavConfig, framesPerBuffer));
}
