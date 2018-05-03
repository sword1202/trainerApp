//
// Created by Semyon Tikhonenko on 4/29/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_COMMONAUDIOPLAYER_H
#define VOCALTRAINER_COMMONAUDIOPLAYER_H

#include "AudioPlayer.h"
#include <string>
#include <iostream>

class AudioFilePlayer : public AudioPlayer {
    int bufferSeek;
    mutable std::mutex seekMutex;
protected:
    std::string audioData;

    virtual int getBufferSeek() const override;
    void setBufferSeekIfNotModified(int bufferSeek, int lastBufferSeek);
    virtual void setBufferSeek(int bufferSeek) override;

public:
    AudioFilePlayer(std::string&& audioData);
    // mp3 and wav files are supported
    static AudioFilePlayer* create(std::istream& is);
    static AudioFilePlayer* create(const char* filePath);
};


#endif //VOCALTRAINER_COMMONAUDIOPLAYER_H
