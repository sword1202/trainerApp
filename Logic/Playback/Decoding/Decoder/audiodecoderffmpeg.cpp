//
// Created by Semyon Tikhonenko on 8/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <iostream>
#include <assert.h>

#include "audiodecoderffmpeg.h"

constexpr int PROBE_BUFFER_SIZE = 8192; // Minumum size to detect format
constexpr AVSampleFormat SAMPLE_FORMAT = AV_SAMPLE_FMT_S16P;

// Callbacks
static int ffmpeg_read(void *data, uint8_t *buf, int size)
{
    AudioDecoderFFmpeg *decoder = static_cast<AudioDecoderFFmpeg*>(data);
    int readedBytes = static_cast<int>(std::min(decoder->input()->tellp() - decoder->input()->tellg(), static_cast<long>(size)));
    decoder->input()->read(reinterpret_cast<char*>(buf), size);
    return readedBytes;
}

static long ffmpeg_seek(void *data, long offset, int whence)
{
    AudioDecoderFFmpeg *decoder = static_cast<AudioDecoderFFmpeg*>(data);
    long position = 0;

    switch(whence)
    {
    case AVSEEK_SIZE:
        return decoder->input()->tellp();
    case SEEK_SET:
        position = offset;
        break;
    case SEEK_CUR:
        position = decoder->input()->tellg() + offset;
        break;
    case SEEK_END:
        position = decoder->input()->tellp() - offset;
        break;
    default:
        return -1;
    }
    if(position < 0 || position > decoder->input()->tellp())
        return -1;


    decoder->input()->seekg(position);
    return position;
}

AudioDecoderFFmpeg::AudioDecoderFFmpeg()
{
    tempPacket.size = 0;
    packet.size = 0;
    packet.data = nullptr;

    av_init_packet(&packet);
    av_init_packet(&tempPacket);
}

AudioDecoderFFmpeg::~AudioDecoderFFmpeg()
{
    tempPacket.size = 0;
    if(codecContext)
        avcodec_free_context(&codecContext);
    if (formatContext)
        avformat_free_context(formatContext);
    if(packet.data)
        av_packet_unref(&packet);
    if(streamContext)
        av_free(streamContext);
    if(resampleContext)
        swr_free(&resampleContext);
    if(decodedFrame)
        av_frame_free(&decodedFrame);
    if(resampledFrame)
        av_frame_free(&resampledFrame);
}

void AudioDecoderFFmpeg::open(std::string &&data)
{
    audioData.write(data.c_str(), long(data.size()) + 1);

    formatContext = avformat_alloc_context();

    // Minumum size for probe to detect format
    if(audioData.tellp() < PROBE_BUFFER_SIZE) {
        std::cerr << "Too small buffer size: " << audioData.tellp() << std::endl;
        return;
    }

    // Allocate probe buffer
    char probeBufer[PROBE_BUFFER_SIZE + AVPROBE_PADDING_SIZE]; // Need to add padding size
    audioData.read(probeBufer, PROBE_BUFFER_SIZE);
    audioData.seekg(0);

    AVProbeData probeData;
    memset(&probeData, 0, sizeof(probeData));
    probeData.buf_size = PROBE_BUFFER_SIZE;
    probeData.buf = reinterpret_cast<unsigned char*>(&probeBufer[0]);

    AVInputFormat *inputFormat = av_probe_input_format(&probeData, 1);
    if(!inputFormat) {
        std::cerr << "Usupported format" << std::endl;
        return;
    }

    streamContext = avio_alloc_context(inputBuffer, INPUT_BUFFER_SIZE, 0, this, ffmpeg_read, nullptr, ffmpeg_seek);
    if(!streamContext) {
        std::cerr << "Unable to initialize I/O callbacks" << std::endl;
        return;
    }

    streamContext->seekable = true;
    streamContext->max_packet_size = INPUT_BUFFER_SIZE;
    formatContext->pb = streamContext;

    if(avformat_open_input(&formatContext, nullptr, inputFormat, nullptr) != 0) {
        std::cerr << "Open input failed" << std::endl;
        return;
    }

    avformat_find_stream_info(formatContext, nullptr);
    if(formatContext->pb)
        formatContext->pb->eof_reached = 0;

    formatContext->flags |= AVFMT_FLAG_GENPTS; // Generate missing pts even if it requires parsing future frames

    // Find audio stream
    streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if(streamIndex < 0) {
        std::cerr << "Unable to find audio stream" << std::endl;
        return;
    }

    codecContext = avcodec_alloc_context3(nullptr);
    avcodec_parameters_to_context(codecContext, formatContext->streams[streamIndex]->codecpar);

    // Detect channels
    if (codecContext->channels == 1) {
        codecContext->request_channel_layout = AV_CH_LAYOUT_MONO;
        m_iChannels = codecContext->channels;
    } else {
        codecContext->request_channel_layout = AV_CH_LAYOUT_STEREO;
        m_iChannels = 2;
    }

    // Print information
    av_dump_format(formatContext, 0, nullptr, 0);

    // Set codec
    AVCodec *codec = avcodec_find_decoder(codecContext->codec_id);
    if (!codec) {
        std::cerr << "Unsupported codec for output stream" << std::endl;
        return;
    }
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        std::cerr << "Error while opening codec for output stream" << std::endl;
        return;
    }

    // Decode to 16-bit int
    resampleContext = swr_alloc_set_opts(nullptr,
                                         static_cast<long>(codecContext->channel_layout),
                                         SAMPLE_FORMAT,
                                         codecContext->sample_rate,
                                          static_cast<long>(codecContext->channel_layout),
                                         codecContext->sample_fmt,
                                         codecContext->sample_rate,
                                         0,
                                         nullptr);
    swr_init(resampleContext);

    // Allocate frames
    decodedFrame = av_frame_alloc();
    resampledFrame = av_frame_alloc();
    resampledFrame->channel_layout = codecContext->channel_layout; // Need to set configuration for resampled frame
    resampledFrame->sample_rate = codecContext->sample_rate;
    resampledFrame->format = SAMPLE_FORMAT;

    // Save information
    m_fDuration = static_cast<float>(formatContext->duration) / AV_TIME_BASE;
    m_iSampleRate = codecContext->sample_rate;

    return;
}

void AudioDecoderFFmpeg::seek(int sampleIdx)
{
    assert(sampleIdx > 0);

    long timestamp = (sampleIdx / m_iSampleRate) / m_iChannels * AV_TIME_BASE;
     av_seek_frame(formatContext, -1, timestamp, AVSEEK_FLAG_BACKWARD);
     avcodec_flush_buffers(codecContext);
     av_packet_unref(&packet);
     tempPacket.size = 0;
     m_iPositionInSamples = sampleIdx;
}

int AudioDecoderFFmpeg::read(int size, SAMPLE *destination)
{
    assert(samplesReaded <= samplesAvailable);

    if (samplesAvailable - samplesReaded == 0) {
        // Buffer is over, need to decode next frame
        fillBuffer();
        if (samplesAvailable - samplesReaded == 0)
            return 0; // End of file
    }

    int samplesToCopy = std::min(samplesAvailable - samplesReaded, size);
    int sampleSize = av_get_bytes_per_sample(SAMPLE_FORMAT);

    // Copy samples to destination
    for (int i = 0; i < samplesToCopy; ++i)
        memcpy(destination + i, resampledFrame->extended_data[(i + samplesReaded) % m_iChannels] + (i + samplesReaded) / m_iChannels * sampleSize, static_cast<unsigned>(sampleSize));

    samplesReaded += samplesToCopy;

    // Check if all samples copied
    if (samplesToCopy < size) {
        // Need to decode next frame
        fillBuffer();
        if (samplesAvailable >= size) {
            // Copy remaining samples if available
            int samplesLeft = size - samplesToCopy;
            for(int i = 0; i < samplesLeft; ++i)
                memcpy(destination + samplesToCopy + i, resampledFrame->extended_data[(i + samplesReaded) % m_iChannels] + (i + samplesReaded) / m_iChannels * sampleSize, static_cast<unsigned>(sampleSize));
            samplesReaded += samplesLeft;
            samplesToCopy += samplesLeft;
        }
    }

    m_iPositionInSamples += samplesToCopy;
    return samplesToCopy;
}

std::vector<std::string> AudioDecoderFFmpeg::supportedFileExtensions()
{
    std::vector<std::string> list;
    list.push_back("m4a");
    list.push_back("mp4");
    list.push_back("wma");
    list.push_back("mp3");
    list.push_back("wav");
    list.push_back("aif");
    list.push_back("aiff");
    return list;
}

std::stringstream *AudioDecoderFFmpeg::input()
{
    return &audioData;
}

void AudioDecoderFFmpeg::fillBuffer()
{
    samplesAvailable = 0;

    while(!samplesAvailable) {
        if(!tempPacket.size) {
            if (av_read_frame(formatContext, &packet) < 0) {
                tempPacket.size = 0;
                break;
            }

            tempPacket.size = packet.size;
            tempPacket.data = packet.data;

            if (packet.stream_index != streamIndex) {
                if (packet.data)
                    av_packet_unref(&packet);
                tempPacket.size = 0;
                continue;
            }
        }

        samplesAvailable = decodeFrame();

        if (samplesAvailable < 0) {
            samplesAvailable = 0;
            tempPacket.size = 0;
            continue;
        }

        if (samplesAvailable == 0) {
            if (packet.data)
                av_packet_unref(&packet);
            packet.data = nullptr;
            tempPacket.size = 0;
            continue;
        }
    }

    samplesReaded = 0; // Reset samples position
}

int AudioDecoderFFmpeg::decodeFrame()
{
    int outSize = 0;
    if (packet.stream_index == streamIndex) {
        int err = avcodec_send_packet(codecContext, &tempPacket);
        if (err != 0 && err != AVERROR(EAGAIN) && err != AVERROR(EINVAL)) {
            std::cerr << "Send packet error: " << err << std::endl;
            return -1;
        }

        int l = (err == AVERROR(EAGAIN)) ? 0 : tempPacket.size;

        // Decode frame
        if ((err = avcodec_receive_frame(codecContext, decodedFrame)) < 0) {
            if (err == AVERROR(EAGAIN)) // Try again
                return 0;
            std::cerr << "Receive frame error: " << err << std::endl;
            return -1;
        }

        // Resample frame to 16-bit int format
        if ((err = swr_convert_frame(resampleContext, resampledFrame, decodedFrame)) != 0) {
            std::cerr << "Resample frame error: " << err << std::endl;
            return -1;
        }

        if (resampledFrame->pkt_size)
            outSize = codecContext->channels * resampledFrame->nb_samples;
        else
            outSize = 0;

        if(l < 0)
            return l;

        tempPacket.data += l;
        tempPacket.size -= l;
    }

    if (!tempPacket.size && packet.data)
        av_packet_unref(&packet);

    return outSize;
}
