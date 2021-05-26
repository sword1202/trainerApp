//
// Created by Semyon Tikhonenko on 10/5/18.
//

#include "AudioInputRecorder.h"
#include "MemoryUtils.h"
#include <iostream>

using namespace std;
using namespace CppUtils;

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
        recordedData->getData().resize(static_cast<size_t>(seek));
        recordedData->getData().append(reinterpret_cast<const char*>(data), static_cast<size_t>(size));
    }

    {
        SEEK_LOCK;
        if (this->seek == seek) {
            this->seek += size;
        }
    }
}

AudioDataBufferConstPtr AudioInputRecorder::getRecordedData() const {
    DATA_LOCK;
    return recordedData;
}

int AudioInputRecorder::getSeek() const {
    SEEK_LOCK;
    return seek;
}

void AudioInputRecorder::setSeek(int seek) {
    SEEK_LOCK;
    this->seek = seek;
}

AudioInputRecorder::AudioInputRecorder(): seek(0) {
    this->recordedData.reset(new StdStringAudioDataBuffer());
}

void AudioInputRecorder::clearRecordedData() {
    DATA_LOCK;
    Memory::FillZero(recordedData->getData().data(), static_cast<int>(recordedData->getNumberOfBytes()));
}
