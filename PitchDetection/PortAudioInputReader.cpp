#include "AudioInputReader.h"
#include <portaudio/portaudio.h>
#include "PortAudioUtils.h"
#include <portmixer.h>
#include "Functions.h"
#include <cmath>

#define SAMPLE_RATE 44100

class PortAudioInputReader : public AudioInputReaderWithOutput {
    static int portAudioCallback(const void *inputBuffer,
            void *outputBuffer,
            unsigned long framesPerBuffer,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void *userData ) {
        PortAudioInputReader* self = (PortAudioInputReader*)userData;

        CppUtils::Functions::ExecuteAll(self->callbacks, (int16_t*)inputBuffer, (int)framesPerBuffer);

        if (self->isOutputEnabled()) {
            const float outputVolume = self->getOutputVolume();
            if (outputVolume == 0.0f) {
                memset(outputBuffer, 0, framesPerBuffer * sizeof(int16_t));
            } else if(outputVolume == 1.0f) {
                memcpy(outputBuffer, inputBuffer, framesPerBuffer * sizeof(int16_t));
            } else {
                int16_t* output = static_cast<int16_t *>(outputBuffer);
                const int16_t* input = static_cast<const int16_t *>(inputBuffer);
                for (int i = 0; i < framesPerBuffer; ++i) {
                    output[i] = (int16_t)round(input[i] * outputVolume);
                }
            }
        }

        return paContinue;
    }

    int maximumBufferSize;
    PaStream *stream;
    PxMixer *mixer;
    bool outputEnabled;
    std::atomic<float> outputVolume;
public:
    PortAudioInputReader(int maximumBufferSize, bool outputEnabled) :
    maximumBufferSize(maximumBufferSize),
    outputEnabled(outputEnabled),
    outputVolume(1.0f)
    {
        PaError err = Pa_OpenDefaultStream(&stream,
                1,          /* 1 input channel */
                (int)outputEnabled,
                paInt16,
                SAMPLE_RATE,
                maximumBufferSize,
                portAudioCallback,
                this );
        PortAudio::checkErrors(err);

        mixer = Px_OpenMixer(stream, 0);
        assert(mixer);
    }
    
    ~PortAudioInputReader() {
        PaError err = Pa_CloseStream(stream);
        PortAudio::checkErrors(err);
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

    void setInputVolume(float value) override {
        Px_SetInputVolume(mixer, value);
    }

    float getInputVolume() const override {
        return Px_GetInputVolume(mixer);
    }

    void setOutputVolume(float value) override {
        outputVolume = value;
    }

    float getOutputVolume() const override {
        return outputVolume;
    }

    bool isOutputEnabled() const {
        return outputEnabled;
    }
};

AudioInputReader* CreateDefaultAudioInputReader(int maximumBufferSize) {
    return new PortAudioInputReader(maximumBufferSize, false);
}

AudioInputReaderWithOutput* CreateDefaultAudioInputReaderWithOutput(int maximumBufferSize) {
    return new PortAudioInputReader(maximumBufferSize, true);
}
