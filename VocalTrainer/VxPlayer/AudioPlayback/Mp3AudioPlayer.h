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

class Mp3AudioPlayer : public AudioFilePlayer {
    short tempPcm[MINIMP3_MAX_SAMPLES_PER_FRAME];
    boost::circular_buffer<short> pcm;
    boost::circular_buffer<int> mp3frameBytesCountQueue;
    mp3dec_t mp3d;
    mp3dec_frame_info_t info;
    int bitrate;
    std::mutex pcmMutex;
    int headerOffset;
    std::atomic_bool decodingThreadRunning;
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    void prepareAndProvidePlaybackData(PlaybackData *playbackData) override;
    void decodingThreadCallback(const PlaybackData& playbackData);
public:
    Mp3AudioPlayer(std::string &&audioData);
    virtual ~Mp3AudioPlayer();

    virtual void setSeek(double timeStamp) override;

    double getSeek() const override;
};


#endif //VOCALTRAINER_MP3AUDIOPLAYER_H
