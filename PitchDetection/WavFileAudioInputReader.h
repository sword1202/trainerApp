//
// Created by Semyon Tikhonenko on 8/19/16.
//

#ifndef PITCHDETECTION_WAVFILEAUDIOINPUTREADER_H
#define PITCHDETECTION_WAVFILEAUDIOINPUTREADER_H


#include <thread>
#include "AudioInputReader.h"
#include "WAVFile.h"
#include "WavBufferReader.h"

class WavFileAudioInputReader : public AudioInputReader {
    AudioInputReader::Callback callback;
    std::thread* thread = 0;
    volatile bool running = false;
    WavBufferReader bufferReader;
public:
    WavFileAudioInputReader(std::istream* stream, int sampleSize);

    virtual void setCallback(Callback callback) override;
    virtual void start() override;
    virtual void stop() override;
    virtual int getSampleRate() const override;
    virtual int getMaximumBufferSize() const override;

    virtual ~WavFileAudioInputReader() override;
    void setDestroyStreamOnDestructor(bool value);
};


#endif //PITCHDETECTION_WAVFILEAUDIOINPUTREADER_H
