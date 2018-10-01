//
// Created by Semyon Tikhonenko on 8/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef AUDIODECODERMEDIAFOUNDATION_H
#define AUDIODECODERMEDIAFOUNDATION_H

#include "Decoder/audiodecoder.h"
#include "mfidl.h"
#include "mfreadwrite.h"

class AudioDecoderMediaFoundation : public AudioDecoder {

public:
    AudioDecoderMediaFoundation();
    ~AudioDecoderMediaFoundation();
    void open(std::string &&data);
    void seek(int sampleIdx);
    int read(int size, SAMPLE *buffer);
    std::vector<std::string> supportedFileExtensions();

private:
    void copyFrames(short *dest, size_t *destFrames, const short *src, size_t srcFrames);
    inline long long frameFromMF(long long mf);
    inline long long mfFromFrame(long long frame);

    IMFSourceReader *reader;
    IMFMediaType *audioType;

    wchar_t *fileNameW; // Temp solution

    int nextFrame = 0;
    short *m_leftoverBuffer;
    size_t m_leftoverBufferSize = 0;
    size_t m_leftoverBufferLength = 0;
    int m_leftoverBufferPosition = 0;
    long long mfDuration = 0;
    bool decoding = true;
    bool seeking = false;
    unsigned int m_BitsPerSample;
    short destBufferShort[8192];
};


#endif //AUDIODECODERMEDIAFOUNDATION_H
