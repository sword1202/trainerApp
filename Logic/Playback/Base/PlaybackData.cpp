#include "PlaybackData.h"

PlaybackData::PlaybackData(const WavConfig &wavConfig, int samplesPerBuffer)
: AudioStreamDescription(wavConfig, samplesPerBuffer) {
}
