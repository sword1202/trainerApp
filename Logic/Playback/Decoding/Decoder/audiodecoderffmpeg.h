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

class AudioDecoderFFmpeg : public AudioDecoder {

public:
    ~AudioDecoderFFmpeg() override;

    void open(std::string &&data) override;
    void seek(int sampleIdx) override;
    int read(int samplesCount, SAMPLE *buffer) override;
    std::vector<std::string> supportedFileExtensions() override;

private:
    void fillBuffer();
    bool decodeFrame();

    // Callbacks for FFmpeg
    static int ffmpegRead(void *data, uint8_t *buf, int size);
    static long ffmpegSeek(void *data, long offset, int whence);

    static constexpr int INPUT_BUFFER_SIZE = 16384; // Buffer size needed for streamContext

    AVFormatContext *formatContext = nullptr;
    AVCodecContext *codecContext = nullptr;
    AVIOContext *streamContext = nullptr;
    SwrContext *resampleContext = nullptr;
    AVFrame *decodedFrame = nullptr;
    AVFrame *resampledFrame = nullptr;
    AVPacket packet;

    int streamIndex = 0;
    int samplesAvailable = 0;
    int samplesRead = 0;
    unsigned char inputBuffer[INPUT_BUFFER_SIZE + AV_INPUT_BUFFER_PADDING_SIZE]; // Need for streamContext
    std::stringstream audioData;
};


#endif //AUDIODECODERFFMPEG_H
