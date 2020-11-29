//
// Created by Semyon Tikhonenko on 4/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXFILEAUDIOPLAYER_H
#define VOCALTRAINER_VXFILEAUDIOPLAYER_H

#include "StlDebugUtils.h"
#include "BaseRawPcmAudioDataPlayer.h"
#include "VocalPart.h"
#include "SfzFile.h"
#include "AudioData.h"
#include "SoundTouch/SoundTouch.h"
#include "PeriodicallySleepingBackgroundTask.h"
#include <atomic>
#include <optional>
#include <queue>

class VocalPartAudioPlayer : public BaseRawPcmAudioDataPlayer {
    VocalPart originalVocalPart;
    VocalPart vocalPart;
    std::map<int, AudioData*> pitchesAudioDataMap;
    std::mutex pitchesAudioDataMapMutex;
    std::mutex updateAudioDataMutex;
    // Needed for fast synchronization
    std::queue<std::vector<short>*> audioDataPool;
    std::mutex audioDataMutex;
    const SfzFile* sfz = nullptr;
    soundtouch::SoundTouch soundTouch;
    std::vector<short> tempPitchBuffer;
    std::vector<float> tempFloatSamplesBuffer;
    std::optional<WavConfig> wavConfig;

    void updateAudioData();
    void updateAudioDataOnBackground();
    size_t getAudioDataBufferSize() const;
protected:
    void providePlaybackData(PlaybackData *playbackData) override;
    void onTonalityChanged(int pitchShift) override;
    int getAudioDataSizeInBytes() override;
    const char *provideAudioBuffer() override;

    int readNextSamplesBatch(void *intoBuffer, int framesCount, const PlaybackData &playbackData) override;

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
