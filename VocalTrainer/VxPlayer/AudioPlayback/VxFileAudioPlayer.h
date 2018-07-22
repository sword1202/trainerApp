//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXFILEAUDIOPLAYER_H
#define VOCALTRAINER_VXFILEAUDIOPLAYER_H

#include "StlDebugUtils.h"
#include "AudioPlayer.h"
#include "VxFile.h"
#include "VxFileAudioDataGenerator.h"
#include "PeriodicallySleepingBackgroundTask.h"
#include <atomic>

class VxFileAudioPlayer : public AudioPlayer {
    VxFileAudioDataGenerator* generator;
    CppUtils::PeriodicallySleepingBackgroundTaskWithCallbacksQueue* generatorTask;
    VxFile originalVxFile;
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    void prepareAndProvidePlaybackData(PlaybackData *playbackData) override;
    void onComplete() override;
    int getBufferSeek() const override;
private:
    void setBufferSeek(int samplesCountSeek) override;

public:
    VxFileAudioPlayer();
    virtual ~VxFileAudioPlayer();

    bool isPitchShiftingAvailable(int distance) const;
    virtual void setPitchShiftInSemiTones(int value) override;

    void setTempoFactor(double tempoFactor) override;

    const VxFile& getVxFile() const;
    void setVxFile(const VxFile& vxFile);
};


#endif //VOCALTRAINER_VXFILEAUDIOPLAYER_H
