//
// Created by Semyon Tikhonenko on 7/23/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_BASEWAVAUDIOPLAYER_H
#define VOCALTRAINER_BASEWAVAUDIOPLAYER_H

#include "AudioPlayer.h"
#include "AudioPlayerWithDefaultSeekHandler.h"

class BaseWavAudioPlayer : public AudioPlayerWithDefaultSeekHandler {
protected:
    void prepareAndProvidePlaybackData(PlaybackData *playbackData) override;

    struct WavSetupData {
        const char* wavHeaderData;
        int size;

        WavSetupData(const char *data, int size);
    };

    virtual WavSetupData provideWavSetupData() = 0;
    virtual double calculateTotalDurationInSeconds(int size, int bytesPerChannel, const PlaybackData& playbackData);
public:
    BaseWavAudioPlayer();
    ~BaseWavAudioPlayer();
};


#endif //VOCALTRAINER_BASEWAVAUDIOPLAYER_H
