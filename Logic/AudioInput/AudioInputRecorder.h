//
// Created by Semyon Tikhonenko on 10/5/18.
//

#ifndef VOCALTRAINER_AUDIOINPUTDATACOLLECTOR_H
#define VOCALTRAINER_AUDIOINPUTDATACOLLECTOR_H

#include <stdint.h>
#include <string>
#include <mutex>

class AudioInputRecorder {
public:
    void operator()(const int16_t* data, int size);
    const std::string &getRecordedData() const;

    int getSeek() const;
    void setSeek(int seek);

    AudioInputRecorder();

private:
    std::string recordedData;
    int seek;
    mutable std::mutex seekMutex;
    mutable std::mutex dataMutex;
};


#endif //VOCALTRAINER_AUDIOINPUTDATACOLLECTOR_H
