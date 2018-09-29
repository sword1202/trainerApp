#include "PortAudioInputReader.h"
#include <portaudio/portaudio.h>
#include "PortAudioUtils.h"
#include "Functions.h"
#include <cmath>

#define SAMPLE_RATE 44100

int PortAudioInputReader::portAudioCallback(const void *inputBuffer,
        void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo *timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData) {
    PortAudioInputReader *self = (PortAudioInputReader *) userData;

    CppUtils::Functions::ExecuteAll(self->callbacks, (int16_t *) inputBuffer, (int) framesPerBuffer);

    if (self->isOutputEnabled()) {
        const float outputVolume = self->getOutputVolume();
        if (outputVolume == 0.0f) {
            memset(outputBuffer, 0, framesPerBuffer * sizeof(int16_t));
        } else if (outputVolume == 1.0f) {
            memcpy(outputBuffer, inputBuffer, framesPerBuffer * sizeof(int16_t));
        } else {
            int16_t *output = static_cast<int16_t *>(outputBuffer);
            const int16_t *input = static_cast<const int16_t *>(inputBuffer);
            for (int i = 0; i < framesPerBuffer; ++i) {
                output[i] = (int16_t) round(input[i] * outputVolume);
            }
        }
    }

    return paContinue;
}

PortAudioInputReader::PortAudioInputReader(int maximumBufferSize, bool outputEnabled, int deviceIndex) :
        maximumBufferSize(maximumBufferSize),
        outputEnabled(outputEnabled),
        outputVolume(1.0f) {
    PaError err = 0;
    if (deviceIndex < 0) {
        err = Pa_OpenDefaultStream(&stream,
                1,          /* 1 input channel */
                (int) outputEnabled,
                paInt16,
                SAMPLE_RATE,
                maximumBufferSize,
                portAudioCallback,
                this);
    } else {
        PaStreamParameters inputParameters;
        inputParameters.sampleFormat = paInt16;
        inputParameters.channelCount = 1;
        inputParameters.device = deviceIndex;
        inputParameters.suggestedLatency = Pa_GetDeviceInfo(deviceIndex)->defaultHighInputLatency;

        PaStreamParameters outputParameters;
        outputParameters.sampleFormat = paInt16;
        outputParameters.channelCount = 1;
        outputParameters.device = deviceIndex;
        outputParameters.suggestedLatency = Pa_GetDeviceInfo(deviceIndex)->defaultHighOutputLatency;

        err = Pa_OpenStream(&stream, &inputParameters,
                &outputParameters,
                SAMPLE_RATE,
                maximumBufferSize,
                paNoFlag,
                portAudioCallback,
                this);
    }

    PortAudio::checkErrors(err);

    mixer = Px_OpenMixer(stream, 0);
    assert(mixer);
}

PortAudioInputReader::~PortAudioInputReader() {
    PaError err = Pa_CloseStream(stream);
    PortAudio::checkErrors(err);
    Px_CloseMixer(mixer);
}

void PortAudioInputReader::start() {
    Pa_StartStream(stream);
}

void PortAudioInputReader::stop() {
    Pa_StopStream(stream);
}

int PortAudioInputReader::getSampleRate() const {
    return SAMPLE_RATE;
}

int PortAudioInputReader::getMaximumBufferSize() const {
    return maximumBufferSize;
}

void PortAudioInputReader::setInputVolume(float value) {
    Px_SetInputVolume(mixer, value);
}

float PortAudioInputReader::getInputVolume() const {
    return Px_GetInputVolume(mixer);
}

void PortAudioInputReader::setOutputVolume(float value) {
    outputVolume = value;
}

float PortAudioInputReader::getOutputVolume() const {
    return outputVolume;
}

bool PortAudioInputReader::isOutputEnabled() const {
    return outputEnabled;
}
