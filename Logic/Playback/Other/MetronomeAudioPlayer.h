//
// Created by Semyon Tikhonenko on 7/23/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_METRONOMEAUDIOPLAYER_H
#define VOCALTRAINER_METRONOMEAUDIOPLAYER_H

#include "WavAudioPlayer.h"
#include "StlContainerAudioDataBuffer.h"
#include <string>

class MetronomeAudioPlayer : public BaseRawPcmAudioDataPlayer, private StdStringAudioDataBuffer {
    double beatsPerMinute = 0;
    static std::string metronomeAudioData;
    int totalVirtualBufferSize;
public:
    MetronomeAudioPlayer();

    static void setMetronomeAudioData(std::string&& metronomeAudioData);

    double getBeatsPerMinute() const;
    void setAudioDataInfo(double beatsPerMinute, double totalDurationInSeconds);

protected:
    virtual void providePlaybackData(PlaybackData *playbackData) override;

protected:
    int getAudioDataSizeInBytes() const override;
    int readAudioData(void *into, int offset, int numberOfBytes) override;

private:
    // StdStringAudioDataBuffer
    int read(void *into, int offset, int numberOfBytes);
};


#endif //VOCALTRAINER_METRONOMEAUDIOPLAYER_H
