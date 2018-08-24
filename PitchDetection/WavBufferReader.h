//
// Created by Semyon Tikhonenko on 9/17/16.
//

#ifndef PITCHDETECTION_WAVBUFFERREADER_H
#define PITCHDETECTION_WAVBUFFERREADER_H

#include "CppUtils/stlassert.h"
#include <vector>
#include "WAVFile.h"

class WavBufferReader {
    WAVFile wavFile;
    int sampleSize;

    std::vector<int16_t> resultBuffer;
    void* tempBuffer;
    int tempBufferSize;
public:
    WavBufferReader(std::istream* stream, int sampleSize);
    ~WavBufferReader();
    int readDataIntoResultBuffer();
    const int16_t *getResultBuffer() const;

    const WAVFile &getWavFile() const;

    int getSampleSize() const;
    int getBufferSize() const;
    void setDestroyStreamOnDestructor(bool value);
};


#endif //PITCHDETECTION_WAVBUFFERREADER_H
