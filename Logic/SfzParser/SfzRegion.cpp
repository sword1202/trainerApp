#include "SfzRegion.h"
#include "WAVFile.h"

const char* SfzRegion::getRawPcmPtr() const {
    return audioData.data() + WAVFile::DATA_POSITION;
}

int SfzRegion::getRawPcmBytesCount() const {
    return static_cast<int>(audioData.size()) - WAVFile::DATA_POSITION;
}
