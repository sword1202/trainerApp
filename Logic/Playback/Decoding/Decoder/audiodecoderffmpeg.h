//
// Created by Semyon Tikhonenko on 8/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef AUDIODECODERFFMPEG_H
#define AUDIODECODERFFMPEG_H

extern "C"{
#include <libavformat/avformat.h>
#include "libswresample/swresample.h"
}

#include <sstream>

#include "audiodecoder.h"

#define INPUT_BUFFER_SIZE 16384

class AudioDecoderFFmpeg : public AudioDecoder {

public:
    AudioDecoderFFmpeg();
    ~AudioDecoderFFmpeg() override;

    void open(std::string &&data) override;
    void seek(int sampleIdx) override;
    int read(int size, SAMPLE *buffer) override;
    std::vector<std::string> supportedFileExtensions() override;

    std::stringstream *input(); // Need for streamContext callbacks

private:
    // Helper functions
    void fillBuffer();
    int decodeFrame();

    AVFormatContext *formatContext = nullptr;
    AVCodecContext *codecContext = nullptr;
    AVIOContext *streamContext = nullptr;
    SwrContext *resampleContext = nullptr;
    AVFrame *decodedFrame = nullptr;
    AVFrame *resampledFrame = nullptr;
    AVPacket packet;
    AVPacket tempPacket;

    int streamIndex = 0;
    int samplesAvailable = 0;
    int samplesReaded = 0;
    unsigned char inputBuffer[INPUT_BUFFER_SIZE + AV_INPUT_BUFFER_PADDING_SIZE]; // Need for streamContext
    std::stringstream audioData;
};


#endif //AUDIODECODERFFMPEG_H
