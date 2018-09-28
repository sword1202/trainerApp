#include "AudioInputReader.h"
#include <portaudio/portaudio.h>
#include "PortAudioUtils.h"
#include <portmixer.h>
#include "Functions.h"

#define SAMPLE_RATE 44100

class PortAudioInputReader : public AudioInputReader {
    static int portAudioCallback(const void *inputBuffer,
            void *outputBuffer,
            unsigned long framesPerBuffer,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void *userData ) {
        PortAudioInputReader* self = (PortAudioInputReader*)userData;

        CppUtils::Functions::ExecuteAll(self->callbacks, (int16_t*)inputBuffer, (int)framesPerBuffer);

        std::memcpy(outputBuffer, inputBuffer, framesPerBuffer * sizeof(int16_t));

        return paContinue;
    }

    int maximumBufferSize;
    PaStream *stream;
    PxMixer *mixer;
public:
    PortAudioInputReader(int maximumBufferSize) : maximumBufferSize(maximumBufferSize) {
        PaError err = Pa_OpenDefaultStream(&stream,
                1,          /* 1 input channel */
                1,          /* no output */
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
};

AudioInputReader* CreateDefaultAudioInputReader(int maximumBufferSize) {
    return new PortAudioInputReader(maximumBufferSize);
}
