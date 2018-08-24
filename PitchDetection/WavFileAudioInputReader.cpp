//
// Created by Semyon Tikhonenko on 8/19/16.
//

#include <cmath>
#include "WavFileAudioInputReader.h"
#include "AudioUtils.h"

#ifdef __APPLE__
#import "CppUtils/TimeUtils.h"
#else
#include "CppUtils/TimeUtils.h"
#endif

using namespace CppUtils::TimeUtils;

void WavFileAudioInputReader::setCallback(AudioInputReader::Callback callback) {
    this->callback = callback;
}

void WavFileAudioInputReader::start() {
    if (thread == 0) {
        int64_t frameTimeMicroSeconds = AudioUtils::GetSampleTimeInMicroseconds(bufferReader.getSampleSize(),
                bufferReader.getWavFile().getSampleRate());
        running = true;
        thread = new std::thread([=]() {
            auto lastFrameTime = NowInMicroseconds();
            while (running) {
                auto nowTime = NowInMicroseconds();
                auto elapsed = nowTime - lastFrameTime;
                if (elapsed > frameTimeMicroSeconds) {
                    lastFrameTime = lastFrameTime + frameTimeMicroSeconds;
                    int count = bufferReader.readDataIntoResultBuffer();
                    if (count > 0) {
                        callback(bufferReader.getResultBuffer(), count);
                    } else {
                        running = false;
                        return;
                    }
                }

                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        });
    }
}

void WavFileAudioInputReader::stop() {
    running = false;
}

int WavFileAudioInputReader::getSampleRate() const {
    return bufferReader.getWavFile().getSampleRate();
}

int WavFileAudioInputReader::getMaximumBufferSize() const {
    return bufferReader.getBufferSize();
}

WavFileAudioInputReader::~WavFileAudioInputReader() {
    if (thread != 0) {
        delete thread;
    }
}

WavFileAudioInputReader::WavFileAudioInputReader(std::istream* stream, int sampleSize) :
        bufferReader(stream, sampleSize) {
    thread = 0;
}

void WavFileAudioInputReader::setDestroyStreamOnDestructor(bool value) {
    bufferReader.setDestroyStreamOnDestructor(value);
}
