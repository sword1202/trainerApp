//
// Created by Semyon Tikhonenko on 7/2/18.
//

#include "AubioPitchDetector.h"
#include "AudioUtils.h"
#include <cassert>

using namespace CppUtils;

static const char* methodString(AubioPitchDetector::Method method) {
    switch (method) {
        case AubioPitchDetector::YIN: return "yin";
        case AubioPitchDetector::YINFFT: return "yinfft";
        case AubioPitchDetector::SCHMITT: return "schmitt";
        case AubioPitchDetector::FCOMB: return "fcomb";
        case AubioPitchDetector::MCOMB: return "mcomb";
        case AubioPitchDetector::SPECACF: return "specacf";
        default: assert(false);
            return "";
    }
}

void AubioPitchDetector::init(int maxBufferSize, int sampleRate) {
    aubio = new_aubio_pitch(methodString(method), maxBufferSize, maxBufferSize, sampleRate);
    in = new_fvec(maxBufferSize);
    onInitQueue.executeAll();
}

float AubioPitchDetector::getFrequencyFromBuffer(const int16_t *buffer) {
    fvec_t fvec;
    float out[1];
    fvec.data = out;
    fvec.length = 1;

    AudioUtils::Int16SamplesIntoFloatSamples(buffer, in->length, in->data);
    aubio_pitch_do(aubio, in, &fvec);
    return out[0];
}

float AubioPitchDetector::getThreshold() const {
    return aubio_pitch_get_tolerance(aubio);
}

void AubioPitchDetector::setThreshold(float threshold) {
    onInitQueue.executeWhenInitialised([=] {
        aubio_pitch_set_tolerance(aubio, threshold);
    });
}

bool AubioPitchDetector::hasThreshold() const {
    return method == YIN || method == YINFFT;
}

AubioPitchDetector::~AubioPitchDetector() {
    del_aubio_pitch(aubio);
    del_fvec(in);
}

AubioPitchDetector::AubioPitchDetector(AubioPitchDetector::Method method) : method(method), onInitQueue(&aubio) {}

AubioPitchDetector::AubioPitchDetector() : method(YINFFT), onInitQueue(&aubio) {

}
