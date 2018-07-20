//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXFILEAUDIOPLAYER_H
#define VOCALTRAINER_VXFILEAUDIOPLAYER_H

#include "StlDebugUtils.h"
#include "AudioPlayer.h"
#include "VxFile.h"
#include "ParallelVxFileAudioDataGenerator.h"
#include "PeriodicallySleepingBackgroundTask.h"
#include <atomic>

class VxFileAudioPlayer : public AudioPlayer {
    ParallelVxFileAudioDataGenerator* generator;
    CppUtils::PeriodicallySleepingBackgroundTaskWithCallbacksQueue* generatorTask;
    VxFile originalVxFile;
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    void prepareAndProvidePlaybackData(PlaybackData *playbackData) override;
    void onComplete() override;
    virtual ~VxFileAudioPlayer();
    int getBufferSeek() const override;
private:
    void setBufferSeek(int samplesCountSeek) override;

public:
    VxFileAudioPlayer(const VxFile &vxFile);
    void destroy(const std::function<void()>& onDestroyed) override;

    bool isPitchShiftingAvailable(int distance) const;
    virtual void setPitchShiftInSemiTones(int value) override;

    const VxFile& getVxFile() const;
};


#endif //VOCALTRAINER_VXFILEAUDIOPLAYER_H
