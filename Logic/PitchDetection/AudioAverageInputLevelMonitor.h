//
// Created by Semyon Tikhonenko on 9/30/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIOINPUTLEVELMONITOR_H
#define VOCALTRAINER_AUDIOINPUTLEVELMONITOR_H

#include "PortAudioInputReader.h"

class AudioAverageInputLevelMonitor {
public:
    typedef std::function<void(double)> Callback;
private:
    mutable std::vector<float> tempFloatBuffer;
    Callback callback;
public:

    AudioAverageInputLevelMonitor(int bufferSize, const Callback& callback);
    void operator()(const int16_t* data, int size) const;
};


#endif //VOCALTRAINER_AUDIOINPUTLEVELMONITOR_H
