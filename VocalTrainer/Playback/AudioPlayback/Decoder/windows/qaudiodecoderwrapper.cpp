//
// Created by Semyon Tikhonenko on 8/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "qaudiodecoderwrapper.h"

void QAudioDecoderWrapper::open(std::string &&data) {
    buffer.setData(QByteArray::fromStdString(data));
    data.clear();
    data.shrink_to_fit();
    audioDecoder.setSourceDevice(&buffer);
    QAudioFormat audioFormat = audioDecoder.audioFormat();
    audioFormat.setSampleSize(sizeof(short));
    audioDecoder.setAudioFormat(audioFormat);
    m_fDuration = audioDecoder.duration() / 1000000.0f;
    m_iChannels = audioFormat.channelCount();
    m_iSampleRate = audioFormat.sampleRate();

    audioDecoder.start();
}

int QAudioDecoderWrapper::seek(int filepos) {
    m_iNumSamples = filepos;
    return buffer.seek(filepos * sizeof(SAMPLE));
}

int QAudioDecoderWrapper::read(int size, SAMPLE *buffer) {
    if (audioDecoder.bufferAvailable()) {
        QAudioBuffer audioBuffer = audioDecoder.read();
        size = std::min(size, audioBuffer.byteCount() / (int)sizeof(SAMPLE));
        memcpy(audioBuffer.data(), buffer, size * sizeof(SAMPLE));
        return size;
    }

    return -1;
}

std::vector<std::string> QAudioDecoderWrapper::supportedFileExtensions() {
    return std::vector<std::string>();
}
