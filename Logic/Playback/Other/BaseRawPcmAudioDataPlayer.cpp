//
// Created by Semyon Tikhonenko on 10/5/18.
//

#include "BaseRawPcmAudioDataPlayer.h"

int BaseRawPcmAudioDataPlayer::readNextSamplesBatch(void *intoBuffer, int framesCount,
                                         const AudioPlayer::PlaybackData &playbackData) {
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

void BaseRawPcmAudioDataPlayer::prepareAndProvidePlaybackData(AudioPlayer::PlaybackData *playbackData) {
    playbackData->totalDurationInSeconds = samplesCountToSeconds(getAudioDataSizeInBytes() / getSampleSize());
}

void BaseRawPcmAudioDataPlayer::setPlaybackData(const WavConfig &wavConfig, int framesPerBuffer) {
    PlaybackData playbackData;
    playbackData.numChannels = wavConfig.numberOfChannels;
    int bytesPerChannel = wavConfig.bitsPerChannel / 8;
    playbackData.framesPerBuffer = framesPerBuffer;
    playbackData.sampleRate = wavConfig.sampleRate;
    switch (bytesPerChannel) {
        case 1:
            playbackData.format = paInt8;
            break;
        case 2:
            playbackData.format = paInt16;
            break;
        case 4:
            playbackData.format = paInt32;
            break;
        default:
            throw std::runtime_error("Unsupported wav bytes per channel " + std::to_string(bytesPerChannel));
    }

    AudioPlayer::setPlaybackData(playbackData);
}
