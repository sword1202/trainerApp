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
#include "SynchronizedCallbacksQueue.h"

class VxFileAudioPlayer : public AudioPlayer {
    const VxFile* vxFile;
    VxFileAudioDataGenerator* generator;
    volatile bool generatorThreadRunning = false;

    void generatorThreadAction();
protected:
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;
    void prepareAndProvidePlaybackData(PlaybackData *playbackData) override;
    void onComplete() override;
private:
    void startGeneratorThread();
    void stopGeneratorThread();
    int getBufferSeek() const override;
    void setBufferSeek(int samplesCountSeek) override;

public:
    VxFileAudioPlayer(const VxFile *vxFile);
    virtual ~VxFileAudioPlayer();
};


#endif //VOCALTRAINER_VXFILEAUDIOPLAYER_H
