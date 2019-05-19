//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXFILEAUDIOPLAYER_H
#define VOCALTRAINER_VXFILEAUDIOPLAYER_H

#include "StlDebugUtils.h"
#include "AudioPlayer.h"
#include "VocalPart.h"
#include "VocalPartAudioDataGenerator.h"
#include "PeriodicallySleepingBackgroundTask.h"
#include <atomic>

class VocalPartAudioPlayer : public AudioPlayer {
    VocalPartAudioDataGenerator* generator;
    VocalPart originalVocalPart;
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    void prepareAndProvidePlaybackData(PlaybackData *playbackData) override;
    void onComplete() override;
    int getBufferSeek() const override;
private:
    void setBufferSeek(int samplesCountSeek) override;

public:
    VocalPartAudioPlayer();
    virtual ~VocalPartAudioPlayer();

    bool isPitchShiftingAvailable(int distance) const;
    virtual void setPitchShiftInSemiTones(int value) override;

    void setTempoFactor(double tempoFactor) override;

    const VocalPart& getVocalPart() const;
    void setVocalPart(const VocalPart &vocalPart);
};


#endif //VOCALTRAINER_VXFILEAUDIOPLAYER_H
