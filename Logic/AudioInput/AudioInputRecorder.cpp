//
// Created by Semyon Tikhonenko on 10/5/18.
//

#include "AudioInputRecorder.h"

#define SEEK_LOCK std::lock_guard<std::mutex> _(seekMutex)
#define DATA_LOCK std::lock_guard<std::mutex> _(dataMutex)

void AudioInputRecorder::operator()(const int16_t *data, int size) {
    int seek;
    {
        SEEK_LOCK;
        seek = this->seek;
    }
    size *= sizeof(int16_t);
    {
        DATA_LOCK;
        collectedData.resize(seek);
        collectedData.append(reinterpret_cast<const char*>(data), size);
    }

    {
        SEEK_LOCK;
        if (this->seek == seek) {
            this->seek += size;
        }
    }
}

const std::string &AudioInputRecorder::getCollectedData() const {
    DATA_LOCK;
    return collectedData;
}

int AudioInputRecorder::getSeek() const {
    SEEK_LOCK;
    return seek;
}

void AudioInputRecorder::setSeek(int seek) {
    SEEK_LOCK;
    this->seek = seek;
}

AudioInputRecorder::AudioInputRecorder(): seek(0) {}
