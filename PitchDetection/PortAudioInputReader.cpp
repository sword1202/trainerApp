#include "AudioInputReader.h"
#include <portaudio/portaudio.h>
#include "PortAudio.h"

#define SAMPLE_RATE 44100

class PortAudioInputReader : public AudioInputReader {
    static int portAudioCallback(const void *inputBuffer,
            void *outputBuffer,
            unsigned long framesPerBuffer,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void *userData ) {
        PortAudioInputReader* self = (PortAudioInputReader*)userData;
        if (self->callback) {
            self->callback((int16_t*)inputBuffer, framesPerBuffer);
        }

        return paContinue;
    }

    int maximumBufferSize;
    PaStream *stream;
    Callback callback;
public:
    PortAudioInputReader(int maximumBufferSize) : maximumBufferSize(maximumBufferSize) {
        PaError err = Pa_OpenDefaultStream(&stream,
                1,          /* 1 input channel */
                0,          /* no output */
                paInt16,
                SAMPLE_RATE,
                maximumBufferSize,
                portAudioCallback,
                this );
        PortAudio::checkErrors(err);
    }
    
    ~PortAudioInputReader() {
        PaError err = Pa_CloseStream(stream);
        PortAudio::checkErrors(err);
    }

    void setCallback(Callback callback) override {
        this->callback = callback;
    }

    void start() override {
        Pa_StartStream(stream);
    }

    void stop() override {
        Pa_StopStream(stream);
    }

    int getSampleRate() const override {
        return SAMPLE_RATE;
    }

    int getMaximumBufferSize() const override {
        return maximumBufferSize;
    }
};

AudioInputReader* CreateDefaultAudioInputReader(int maximumBufferSize) {
    return new PortAudioInputReader(maximumBufferSize);
}

AudioInputReader::~AudioInputReader() {

}