//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_WAVAUDIOPLAYER_H
#define VOCALTRAINER_WAVAUDIOPLAYER_H

#include "AudioFilePlayer.h"
#include "WAVFile.h"

class WavAudioPlayer : public AudioPlayer {
    std::string audioData;
    std::atomic_int bufferSeek;
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    void prepareAndProvidePlaybackData(PlaybackData *playbackData) override;

    int getBufferSeek() const override;
    void setBufferSeek(int bufferSeek) override;
public:
    void setAudioData(std::string&& audioData);

    WavAudioPlayer();
    ~WavAudioPlayer();
};


#endif //VOCALTRAINER_WAVAUDIOPLAYER_H
