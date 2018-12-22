#include "PortAudioInputReader.h"

#include <cstring>
#include <cmath>
#include <portaudio/portaudio.h>

#include "PortAudioUtils.h"
#include "Functions.h"

#define SAMPLE_RATE 44100

int PortAudioInputReader::portAudioCallback(const void *inputBuffer,
        void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo *timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData) {
    PortAudioInputReader *self = (PortAudioInputReader *) userData;

    self->callbacks.executeAll((int16_t *) inputBuffer, (int) framesPerBuffer);

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

PortAudioInputReader::PortAudioInputReader(int maximumBufferSize, bool outputEnabled, const char* deviceName) :
        maximumBufferSize(maximumBufferSize),
        outputEnabled(outputEnabled),
        outputVolume(1.0f),
        deviceName(deviceName ? deviceName : "") {
    init();
}

void PortAudioInputReader::init() {
    int deviceIndex;
    device = PortAudio::findInputDeviceByName(deviceName.data(), &deviceIndex);
    if (device == nullptr) {
        deviceIndex = Pa_GetDefaultInputDevice();
        if (deviceIndex < 0) {
            throw new std::runtime_error("No input devices found");
        }

        device = Pa_GetDeviceInfo(deviceIndex);
        assert(device);
        deviceName = device->name;
    }

    PaStreamParameters inputParameters;
    inputParameters.sampleFormat = paInt16;
    inputParameters.channelCount = 1;
    inputParameters.device = deviceIndex;
    inputParameters.hostApiSpecificStreamInfo = nullptr;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(deviceIndex)->defaultHighInputLatency;

    int outputDeviceIndex = Pa_GetDefaultOutputDevice();
    PaStreamParameters outputParameters;
    if (outputEnabled) {
        outputParameters.sampleFormat = paInt16;
        outputParameters.channelCount = 1;
        outputParameters.device = outputDeviceIndex;
        outputParameters.hostApiSpecificStreamInfo = nullptr;
        outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputDeviceIndex)->defaultHighOutputLatency;
    }

    PaError err = Pa_OpenStream(&stream, &inputParameters,
            outputEnabled ? &outputParameters : nullptr,
            SAMPLE_RATE,
            maximumBufferSize,
            paNoFlag,
            portAudioCallback,
            this);

    PortAudio::checkErrors(err);

    mixer = Px_OpenMixer(stream, 0);
#ifndef __linux__ // Portmixer temporarily does not work on Linux
    assert(mixer);
#endif
}

PortAudioInputReader::~PortAudioInputReader() {
    destroy();
}

void PortAudioInputReader::destroy() {
    if (stream) {
        PaError err = Pa_CloseStream(stream);
        PortAudio::checkErrors(err);
        stream = nullptr;
    }
    if (mixer) {
        Px_CloseMixer(mixer);
        mixer = nullptr;
    }
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

const char* PortAudioInputReader::getDeviceName() const {
    return deviceName.data();
}

void PortAudioInputReader::setDeviceName(const char* deviceName) {
    deviceName = deviceName ? deviceName : "";
    if (deviceName == this->deviceName) {
        return;
    }

    this->deviceName = deviceName;
    bool running = isRunning();
    destroy();
    init();
    if (running) {
        start();
    }
}

bool PortAudioInputReader::isRunning() {
    return stream && Pa_IsStreamActive(stream);
}

int PortAudioInputReader::getSampleSizeInBytes() const {
    return sizeof(int16_t);
}

int PortAudioInputReader::getNumberOfChannels() const {
    return 1;
}

WavConfig PortAudioInputReader::generateWavConfig() const {
    WavConfig wavConfig;
    wavConfig.bitsPerChannel = sizeof(int16_t) * 8;
    wavConfig.numberOfChannels = getNumberOfChannels();
    wavConfig.sampleRate = getSampleRate();
    return wavConfig;
}
