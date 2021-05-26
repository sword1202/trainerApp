#include "AudioDataBuffer.h"

WavConfig AudioDataBuffer::parseWavHeader() const {
    assert(getNumberOfBytes() >= WAVFile::DATA_POSITION);
    char buffer[WAVFile::DATA_POSITION];
    read(buffer, 0, WAVFile::DATA_POSITION);
    return WAVFile::parseWavHeader(static_cast<const char*>(buffer));
}

std::string AudioDataBuffer::toBinaryString() const {
    std::string result;
    result.resize(static_cast<size_t>(getNumberOfBytes()));
    read(result.data(), 0, getNumberOfBytes());
    return result;
}
