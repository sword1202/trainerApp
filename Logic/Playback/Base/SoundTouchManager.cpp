//
// Created by Semyon Tikhonenko on 2018-12-27.
//

#include "SoundTouchManager.h"
#include "AudioUtils.h"

void SoundTouchManager::onPlayBackDataReceived(const PlaybackData &playbackData) {
    soundTouch.setChannels((uint)playbackData.numChannels);
    soundTouch.setSampleRate((uint)playbackData.sampleRate);
    assert(playbackData.framesPerBuffer > 0 && playbackData.numChannels > 0);
    tempFloatBuffer.resize(static_cast<size_t>(playbackData.framesPerBuffer) * playbackData.numChannels);
    this->playbackData = playbackData;
}

void SoundTouchManager::changeTonality(int16_t *data, int framesCount, int shiftInSemiTones) {
    if (shiftInSemiTones == 0) {
        return;
    }

    soundTouch.setPitchSemiTones(shiftInSemiTones);
    int samplesCount = framesCount * playbackData.numChannels;
    AudioUtils::Int16SamplesIntoFloatSamples(data,
            samplesCount,
            tempFloatBuffer.data());
    soundTouch.putSamples(tempFloatBuffer.data(), (uint)framesCount);
    soundTouch.receiveSamples(tempFloatBuffer.data(), (uint)framesCount);
    AudioUtils::FloatSamplesIntoInt16Samples(tempFloatBuffer.data(), samplesCount, data);
}
