//
// Created by Semyon Tikhonenko on 12/5/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOOUTPUTWRITER_H
#define VOCALTRAINER_AUDIOOUTPUTWRITER_H

#include "WAVFile.h"
#include "PlaybackData.h"

class AudioOutputWriter {
public:
    typedef std::function<void(void*, int)> Callback;
    Callback callback;

    virtual ~AudioOutputWriter() = default;
    virtual void start() = 0;
    virtual void stop() = 0;

    static AudioOutputWriter* create(const AudioStreamDescription& data);
};


#endif //VOCALTRAINER_AUDIOOUTPUTWRITER_H
