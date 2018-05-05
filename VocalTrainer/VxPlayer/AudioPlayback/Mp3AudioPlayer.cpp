//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <thread>
#include "Mp3AudioPlayer.h"
#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
#include "Primitives.h"
#include <thread>

#define PCM_LOCK std::lock_guard<std::mutex> _(pcmMutex)

constexpr size_t QUEUE_SIZE = 3;

using namespace CppUtils;

Mp3AudioPlayer::Mp3AudioPlayer(std::string &&audioData) : AudioFilePlayer(std::move(audioData)),
                                                          pcm(MINIMP3_MAX_SAMPLES_PER_FRAME * QUEUE_SIZE),
                                                          mp3frameBytesCountQueue(QUEUE_SIZE) {
}

int Mp3AudioPlayer::readNextSamplesBatch(void *intoBuffer, int framesCount,
        const AudioPlayer::PlaybackData &playbackData) {
    int seek = getBufferSeek();

    int size;
    {
        PCM_LOCK;
        size = pcm.size();
    }

    int outBufferSize = framesCount * playbackData.numChannels;
    if (size < outBufferSize) {
        return -1;
    }

    int newSeek;
    {
        PCM_LOCK;
        short* a = (short*)intoBuffer;
        std::copy(pcm.begin(), pcm.begin() + outBufferSize, a);
        pcm.erase_begin(outBufferSize);
        newSeek = mp3frameBytesCountQueue.front() + seek;
        mp3frameBytesCountQueue.pop_front();
    }

    setBufferSeekIfNotModified(newSeek, seek);

    return framesCount;
}

void Mp3AudioPlayer::prepareAndProvidePlaybackData(AudioPlayer::PlaybackData *playbackData) {
    mp3dec_init(&mp3d);
    memset(&info, 0, sizeof(info));

    int samplesCount = 0;
    int seek = 0;
    do {
        samplesCount = mp3dec_decode_frame(&mp3d, (unsigned char*)audioData.data() + seek,
                audioData.size() - seek, tempPcm, &info);
        seek += info.frame_bytes;
    } while (samplesCount == 0 && info.frame_bytes > 0);

    if (samplesCount == 0) {
        throw std::runtime_error("Corrupted mp3 file");
    }

    playbackData->format = paInt16;
    playbackData->numChannels = info.channels;
    playbackData->sampleRate = info.hz;
    playbackData->framesPerBuffer = MINIMP3_MAX_SAMPLES_PER_FRAME / 2;

    headerOffset = seek - info.frame_bytes;
    size_t mp3StreamBytesCount = audioData.size() - headerOffset;

    bitrate = info.bitrate_kbps;
    double durationInSeconds = mp3StreamBytesCount * 8.0 / bitrate / 1000.0;

    playbackData->totalDurationInSeconds = durationInSeconds;

    pcm.insert(pcm.end(), tempPcm, tempPcm + samplesCount * playbackData->numChannels);
    mp3frameBytesCountQueue.push_back(info.frame_bytes);
    setBufferSeek(0);

    int64_t timeToSleepInMicroseconds = 1000000 * playbackData->framesPerBuffer / playbackData->sampleRate / 10;
    decoderTask.runWithSleepingIntervalInMicroseconds([=]{
        decodingThreadCallback(*playbackData);
    }, timeToSleepInMicroseconds);
}

void Mp3AudioPlayer::decodingThreadCallback(const PlaybackData& playbackData) {
    int pcmSize;
    {
        PCM_LOCK;
        pcmSize = pcm.size();
    }

    while (pcmSize < MINIMP3_MAX_SAMPLES_PER_FRAME * playbackData.numChannels) {
        int seek = getBufferSeek();
        {
            PCM_LOCK;
            for (int i : mp3frameBytesCountQueue) {
                seek += i;
            }
        }

        seek += headerOffset;

        int samplesCount = mp3dec_decode_frame(&mp3d, (unsigned char*)audioData.data() + seek,
                audioData.size() - seek, tempPcm, &info);

        if (samplesCount == 0 && info.frame_bytes > 0) {
            throw std::runtime_error("Corrupted mp3 file");
        }

        if (info.frame_bytes > 0) {
            PCM_LOCK;
            mp3frameBytesCountQueue.push_back(info.frame_bytes);
            pcm.insert(pcm.end(), tempPcm, tempPcm + samplesCount * playbackData.numChannels);
        } else {
            break;
        }

        {
            PCM_LOCK;
            pcmSize = pcm.size();
        }
    }
}

void Mp3AudioPlayer::setSeek(double timeStamp) {
    if (Primitives::CompareFloats(timeStamp, getSeek())) {
        return;
    }

    {
        PCM_LOCK;
        pcm.clear();
        mp3frameBytesCountQueue.clear();
    }

    AudioPlayer::setSeek(timeStamp);
}

double Mp3AudioPlayer::bufferSeekToSecondsSeek(int bufferSeek) const {
    // mp3 formula: durationInSeconds = Size * 8 / bitrate / 1000;
    return bufferSeek * 8.0 / bitrate / 1000.0;
}

int Mp3AudioPlayer::secondsSeekToBufferSeek(double timestamp) const {
    // mp3 formula: durationInSeconds = Size * 8 / bitrate / 1000;
    return (int)round(timestamp * bitrate * 1000.0 / 8.0);
}

void Mp3AudioPlayer::destroy(const std::function<void()>& onDestroy) {
    decoderTask.stop([=] {
        delete this;
        onDestroy();
    });
}
