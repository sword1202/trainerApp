//
// Created by Semyon Tikhonenko on 8/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_QAUDIODECODERWRAPPER_H
#define VOCALTRAINER_QAUDIODECODERWRAPPER_H

#include "Decoder/audiodecoder.h"
#include <QBuffer>
#include <QAudioDecoder>

class QAudioDecoderWrapper : public AudioDecoder {
public:
    void open(std::string &&data) override;
    int seek(int filepos) override;
    int read(int size, SAMPLE *buffer) override;
    std::vector<std::string> supportedFileExtensions() override;
private:
    QBuffer buffer;
    QAudioDecoder audioDecoder;
};


#endif //VOCALTRAINER_QAUDIODECODERWRAPPER_H
