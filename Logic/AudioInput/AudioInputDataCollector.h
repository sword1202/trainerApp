//
// Created by Semyon Tikhonenko on 10/5/18.
//

#ifndef VOCALTRAINER_AUDIOINPUTDATACOLLECTOR_H
#define VOCALTRAINER_AUDIOINPUTDATACOLLECTOR_H

#include <stdint.h>
#include <deque>

class AudioInputDataCollector {
public:
    void operator()(const int16_t* data, int size);
    const std::deque<int16_t> &getCollectedData() const;
    void clearCollectedData();
private:
    std::deque<int16_t> collectedData;
};


#endif //VOCALTRAINER_AUDIOINPUTDATACOLLECTOR_H
