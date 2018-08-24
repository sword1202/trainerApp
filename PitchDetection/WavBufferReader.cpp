//
// Created by Semyon Tikhonenko on 9/17/16.
//

#include <assert.h>
#include "WavBufferReader.h"

int WavBufferReader::readDataIntoResultBuffer() {
    if (!wavFile.isValid()) {
        return -1;
    }

    int count = wavFile.readData(tempBuffer, tempBufferSize);
    int bytesPerChannel = wavFile.getBytesPerChannel();
    int size = count / bytesPerChannel;
    for (int i = 0; i < size;) {
        int sum = 0;
        switch (bytesPerChannel) {
            case 1:
            {
                int indexInResultBuffer = i;
                for (int j = 0; j < wavFile.getNumberOfChannels(); ++j) {
                    assert(i < tempBufferSize);
                    sum += ((char*)tempBuffer)[i++];
                }
                sum /= wavFile.getNumberOfChannels();
                sum = INT16_MAX / INT8_MAX * sum;
                resultBuffer[indexInResultBuffer / wavFile.getNumberOfChannels()] = (int16_t) sum;
                break;
            }
            case 2:
            {
                int indexInResultBuffer = i;
                for (int j = 0; j < wavFile.getNumberOfChannels(); ++j) {
                    assert(i < tempBufferSize / 2);
                    sum += ((int16_t*)tempBuffer)[i++];
                }
                sum /= wavFile.getNumberOfChannels();
                resultBuffer[indexInResultBuffer / wavFile.getNumberOfChannels()] = (int16_t) sum;
                break;
            }
            case 3:
                throw std::runtime_error("3 bytes .wav is not supported");
            case 4:
            {
                int indexInResultBuffer = i;
                for (int j = 0; j < wavFile.getNumberOfChannels(); ++j) {
                    assert(i < tempBufferSize / 4);
                    sum += ((int32_t*)tempBuffer)[i++];
                }
                sum /= wavFile.getNumberOfChannels();
                sum = INT16_MAX / INT32_MAX * sum;
                resultBuffer[indexInResultBuffer / wavFile.getNumberOfChannels()] = (int16_t) sum;
                break;
            }
            default:
                throw std::runtime_error("only 1, 2, 4 bytes .wav are supported");
        }
    }

    return count / wavFile.getNumberOfChannels() / bytesPerChannel;
}

WavBufferReader::WavBufferReader(std::istream* stream, int sampleSize) : wavFile(stream),
                                                                           sampleSize(sampleSize) {
    assert(sampleSize > 0);
    resultBuffer.resize((size_t) sampleSize);
    tempBufferSize = sampleSize * wavFile.getBytesPerChannel() *
                     wavFile.getNumberOfChannels();
    tempBuffer = malloc((size_t) tempBufferSize);
}

const int16_t * WavBufferReader::getResultBuffer() const {
    return resultBuffer.data();
}

WavBufferReader::~WavBufferReader() {
    free(tempBuffer);
}

const WAVFile &WavBufferReader::getWavFile() const {
    return wavFile;
}

int WavBufferReader::getSampleSize() const {
    return sampleSize;
}

int WavBufferReader::getBufferSize() const {
    return getSampleSize() * wavFile.getNumberOfChannels();
}

void WavBufferReader::setDestroyStreamOnDestructor(bool value) {
    wavFile.setDeleteStreamOnDestructor(value);
}
