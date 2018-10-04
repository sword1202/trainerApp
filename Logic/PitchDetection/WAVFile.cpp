//
// Created by Semyon Tikhonenko on 8/19/16.
//

#include <assert.h>
#include "WAVFile.h"
#include <iostream>

#ifndef NO_BOOST
#include <boost/iostreams/stream.hpp>
#endif

static const int SAMPLE_RATE_SIZE = 4;
static const int SAMPLE_RATE_POSITION = 24;
static const int CHANNELS_NUMBER_SIZE = 2;
static const int CHANNELS_NUMBER_POSITION = 22;
static const int BITS_PER_CHANNEL_SIZE = 2;
static const int BITS_PER_CHANNEL_POSITION = 34;
static const int AUDIO_FORMAT_POSITION = 20;

WAVFile::WAVFile(std::istream *stream) {
    init(stream);
}

void WAVFile::init(std::istream *stream) {
    assert(*stream && " stream is invalid");
    this->stream = stream;
    stream->seekg(CHANNELS_NUMBER_POSITION, std::ios_base::beg);
    stream->read((char*)&numberOfChannels, CHANNELS_NUMBER_SIZE);
    stream->seekg(SAMPLE_RATE_POSITION, std::ios_base::beg);
    stream->read((char*)&sampleRate, SAMPLE_RATE_SIZE);
    int bitsPerChannel = 0;
    stream->seekg(BITS_PER_CHANNEL_POSITION, std::ios_base::beg);
    stream->read((char*)&bitsPerChannel, BITS_PER_CHANNEL_SIZE);
    assert(bitsPerChannel >= 8);
    bytesPerChannel = bitsPerChannel / 8;
    stream->seekg(AUDIO_FORMAT_POSITION, std::ios_base::beg);
    stream->read((char*)&audioFormat, sizeof(short));
    moveToTheFirstDataByte();
}

#ifndef NO_BOOST

WAVFile::WAVFile(const char *array, int size) {
    stream =  new boost::iostreams::stream<boost::iostreams::array_source>(array, size);
    deleteStreamOnDestructor = true;
    init(stream);
}

#endif

bool WAVFile::isValid() const {
    return !stream->fail();
}

int WAVFile::getNumberOfChannels() const {
    return numberOfChannels;
}

int WAVFile::getBytesPerChannel() const {
    return bytesPerChannel;
}

int WAVFile::getSampleRate() const {
    return sampleRate;
}

void WAVFile::moveToTheFirstDataByte() {
    stream->seekg(DATA_POSITION, std::ios::beg);
}

int WAVFile::readData(void *buf, int size) {
    stream->read((char*)buf, size);
    return (int) stream->gcount();
}

int WAVFile::readData(void *buf, int size, int seek) {
    stream->seekg(DATA_POSITION + seek, std::ios::beg);
    stream->read((char*)buf, size);
    return (int) stream->gcount();
}

bool WAVFile::shouldDeleteStreamOnDestructor() const {
    return deleteStreamOnDestructor;
}

void WAVFile::setDeleteStreamOnDestructor(bool deleteStreamOnDestructor) {
    WAVFile::deleteStreamOnDestructor = deleteStreamOnDestructor;
}

WAVFile::~WAVFile() {
    if (deleteStreamOnDestructor) {
        delete stream;
    }
}

std::vector<char> WAVFile::addWavHeaderToRawPcmData(const char *data, int size, const WavConfig &config) {
    unsigned long totalDataLen = static_cast<unsigned long>(size + DATA_POSITION);
    unsigned long sampleRate = config.sampleRate;
    unsigned int channels = config.numberOfChannels;
    unsigned long byteRate = (config.bitsPerChannel * sampleRate * channels) / 8;
    
    std::vector<char> result(size + DATA_POSITION);
    result[0] = 'R';  // RIFF/WAVE header
    result[1] = 'I';
    result[2] = 'F';
    result[3] = 'F';
    result[4] = (char) (totalDataLen & 0xff);
    result[5] = (char) ((totalDataLen >> 8) & 0xff);
    result[6] = (char) ((totalDataLen >> 16) & 0xff);
    result[7] = (char) ((totalDataLen >> 24) & 0xff);
    result[8] = 'W';
    result[9] = 'A';
    result[10] = 'V';
    result[11] = 'E';
    result[12] = 'f';  // 'fmt ' chunk
    result[13] = 'm';
    result[14] = 't';
    result[15] = ' ';
    result[16] = 16;  // 4 bytes: size of 'fmt ' chunk
    result[17] = 0;
    result[18] = 0;
    result[19] = 0;
    result[20] = 1;  // format = 1 for pcm and 2 for byte integer
    result[21] = 0;
    result[22] = (char) channels;
    result[23] = 0;
    result[24] = (char) (sampleRate & 0xff);
    result[25] = (char) ((sampleRate >> 8) & 0xff);
    result[26] = (char) ((sampleRate >> 16) & 0xff);
    result[27] = (char) ((sampleRate >> 24) & 0xff);
    result[28] = (char) (byteRate & 0xff);
    result[29] = (char) ((byteRate >> 8) & 0xff);
    result[30] = (char) ((byteRate >> 16) & 0xff);
    result[31] = (char) ((byteRate >> 24) & 0xff);
    result[32] = (char) (16*1)/8;  // block align
    result[33] = 0;
    result[34] = static_cast<char>(config.bitsPerChannel);  // bits per sample
    result[35] = 0;
    result[36] = 'd';
    result[37] = 'a';
    result[38] = 't';
    result[39] = 'a';
    result[40] = (char) (size & 0xff);
    result[41] = (char) ((size >> 8) & 0xff);
    result[42] = (char) ((size >> 16) & 0xff);
    result[43] = (char) ((size >> 24) & 0xff);

    memcpy(result.data() + DATA_POSITION, data, size);

    return result;
}

std::vector<char> WAVFile::addWavHeaderToRawPcmData(const char *data, int size) {
    return addWavHeaderToRawPcmData(data, size, WavConfig());
}

bool WAVFile::isWavFile(const char *data, int size) {
    assert(size >= 0);
    if (size <= DATA_POSITION) {
        return false;
    }

    return data[0] == 'R' &&
    data[1] == 'I' &&
    data[2] == 'F' &&
    data[3] == 'F' && 
    data[8] == 'W' &&
    data[9] == 'A' &&
    data[10] == 'V' &&
    data[11] == 'E' &&
    data[12] == 'f' && 
    data[13] == 'm' &&
    data[14] == 't';
}

int WAVFile::getAudioFormat() const {
    return audioFormat;
}
