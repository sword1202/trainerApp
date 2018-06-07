//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_WAVAUDIOPLAYER_H
#define VOCALTRAINER_WAVAUDIOPLAYER_H

#include "AudioFilePlayer.h"
#include "WAVFile.h"

class WavAudioPlayer : public AudioPlayer {
    WAVFile* wavFile;
    std::string audioData;
    int bufferSeek;
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    void prepareAndProvidePlaybackData(PlaybackData *playbackData) override;
public:
    WavAudioPlayer(std::string &&audioData);
    ~WavAudioPlayer();

protected:
    int getBufferSeek() const override;

    void setBufferSeek(int bufferSeek) override;
};


#endif //VOCALTRAINER_WAVAUDIOPLAYER_H
