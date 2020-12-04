#include "PlaybackData.h"

PlaybackData::PlaybackData(const WavConfig &wavConfig, int framesPerBuffer) {
    this->numChannels = wavConfig.numberOfChannels;
    int bytesPerChannel = wavConfig.bitsPerChannel / 8;
    this->framesPerBuffer = framesPerBuffer;
    this->sampleRate = wavConfig.sampleRate;
    switch (bytesPerChannel) {
        case 1:
            this->format = paInt8;
            break;
        case 2:
            this->format = paInt16;
            break;
        case 4:
            this->format = paInt32;
            break;
        default:
            throw std::runtime_error("Unsupported wav bytes per channel " + std::to_string(bytesPerChannel));
    }
}
