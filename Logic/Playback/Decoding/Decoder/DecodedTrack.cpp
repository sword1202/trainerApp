//
// Created by Semyon Tikhonenko on 2018-12-28.
//

#include "DecodedTrack.h"

void DecodedTrack::initWithWavData(const std::string &wavData) {
    assert(WAVFile::isWavFile(wavData.data(), wavData.size()));
    wavConfig = WAVFile::parseWavHeader(wavData.data());
    rawPcm.assign(wavData.data() + WAVFile::DATA_POSITION, wavData.size() - WAVFile::DATA_POSITION);
}
