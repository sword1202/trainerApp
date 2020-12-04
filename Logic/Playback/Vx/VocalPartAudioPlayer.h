//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXFILEAUDIOPLAYER_H
#define VOCALTRAINER_VXFILEAUDIOPLAYER_H

#include "StlDebugUtils.h"
#include "AudioPlayerWithDefaultSeekHandler.h"
#include "VocalPart.h"
#include "SfzFile.h"
#include "AudioData.h"
#include "SoundTouch/SoundTouch.h"
#include "FixedSizeObjectPool.h"
#include "PeriodicallySleepingBackgroundTask.h"
#include <atomic>
#include <optional>
#include <queue>

class VocalPartAudioPlayer : public AudioPlayerWithDefaultSeekHandler {
    VocalPart originalVocalPart;
    VocalPart vocalPart;
    const SfzFile* sfz = nullptr;

    struct PlayingPitch {
        const SfzRegion* region;
        int vocalPartPitchIndex;
        int bufferSeek = 0;
    };

    std::vector<const short*> soundsToMix;
    std::vector<int> soundsToMixSizes;
    std::vector<float> tempFloatBuffer;
    CppUtils::FixedSizeObjectPool<std::vector<short>> tempShortBufferPool;
    std::vector<PlayingPitch> playingPitches;
    std::mutex playingPitchesMutex;
    soundtouch::SoundTouch soundTouch;

    PlayingPitch* findPlayingPitch(int vocalPartPitchIndex);
protected:
    void onTonalityChanged(int pitchShift) override;
    void providePlaybackData(PlaybackData *playbackData) override;
    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;

    void onBufferSeekChanged(int seekBefore, int seekNow) override;

public:
    VocalPartAudioPlayer();
    virtual ~VocalPartAudioPlayer();

    bool isPitchShiftingAvailable(int distance) const;
    void setSfz(const SfzFile* sfz);
    void setTempoFactor(double tempoFactor) override;
    const VocalPart& getVocalPart() const;
    void setVocalPart(const VocalPart &vocalPart);
};


#endif //VOCALTRAINER_VXFILEAUDIOPLAYER_H
