//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_MP3AUDIOPLAYER_H
#define VOCALTRAINER_MP3AUDIOPLAYER_H

#include "StlDebugUtils.h"
#include "AudioFilePlayer.h"
#include "minimp3.h"
#define BOOST_CB_ENABLE_DEBUG 1
#include <boost/circular_buffer.hpp>
#include <atomic>
#include "PeriodicallySleepingBackgroundTask.h"
#include <SoundTouch/SoundTouch.h>

class Mp3AudioPlayer : public AudioFilePlayer {
    short tempPcm[MINIMP3_MAX_SAMPLES_PER_FRAME];
    float tempPcmFloat[MINIMP3_MAX_SAMPLES_PER_FRAME];
    boost::circular_buffer<short> pcm;
    boost::circular_buffer<int> mp3frameBytesCountQueue;
    mp3dec_t mp3d;
    mp3dec_frame_info_t info;
    int bitrate;
    std::mutex pcmMutex;
    int headerOffset;
    soundtouch::SoundTouch soundTouch;
    CppUtils::PeriodicallySleepingBackgroundTask decoderTask;
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    void prepareAndProvidePlaybackData(PlaybackData *playbackData) override;
    void decodingThreadCallback(const PlaybackData& playbackData);
public:
    Mp3AudioPlayer(std::string &&audioData);
    virtual void setSeek(double timeStamp) override;

    void destroy(const std::function<void()>& onDestroyed) override;

protected:
    double bufferSeekToSecondsSeek(int bufferSeek) const override;
    int secondsSeekToBufferSeek(double timestamp) const override;

public:
    void setPitchShiftInSemiTones(int value) override;
};


#endif //VOCALTRAINER_MP3AUDIOPLAYER_H
