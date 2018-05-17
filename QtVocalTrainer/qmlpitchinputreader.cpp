#include "qmlpitchinputreader.h"
#include "../PitchDetection/CppUtils/TimeUtils.h"
#include "../PitchDetection/CppUtils/Executors.h"
#include "app.h"

static const int PITCH_DETECTION_BUFFER_SIZE = 1200;
static const int PITCH_SMOOTH_LEVEL = 4;

QmlPitchInputReader::QmlPitchInputReader(QObject *parent) : QObject(parent) {
    audioInputReader = CreateDefaultAudioInputReader(PITCH_DETECTION_BUFFER_SIZE);
    pitchInputReader = new PitchInputReader(audioInputReader, PITCH_SMOOTH_LEVEL);
    pitchInputReader->setExecuteCallBackOnInvalidPitches(true);
    pitchInputReader->setCallback([=](const Pitch& pitch) {
        float frequency = pitch.getFrequency();
        double time = TimeUtils::NowInSeconds();

        App::instance()->executeOnMainThread([=] {
            if (!frequencies.empty() && frequencies.back() < 0 && frequency < 0) {
                return;
            }

            frequencies.push_back(frequency);
            times.push_back(time);
            if (times.back() - times.front() > savedPitchesTimeLimit) {
                times.pop_front();
                frequencies.pop_front();
            }

            emit pitchDetected(QmlPitch(frequency, time));
        });
    });
}

QmlPitchInputReader::~QmlPitchInputReader() {
    if (pitchInputReader) {
        delete pitchInputReader;
    }
}

void QmlPitchInputReader::start() {
    frequencies.clear();
    times.clear();
    running = true;
    pitchInputReader->start();
}

void QmlPitchInputReader::stop() {
    running = false;
    pitchInputReader->stop();
}

int QmlPitchInputReader::getPitchesCount() {
    return frequencies.size();
}

QmlPitch QmlPitchInputReader::getPitchAt(int index) {
    assert(index >= 0 && index < frequencies.size());
    return QmlPitch(frequencies[index], times[index]);
}

bool QmlPitchInputReader::isRunning() const {
    return running;
}


void QmlPitchInputReader::setThreshold(float threshold) {
    pitchInputReader->setThreshold(threshold);
}

float QmlPitchInputReader::getThreshold() const {
    return pitchInputReader->getThreshold();
}

double QmlPitchInputReader::getSavedPitchesTimeLimit() {
    return savedPitchesTimeLimit;
}

void QmlPitchInputReader::setSavedPitchesTimeLimit(double savedPitchesTimeLimit) {
    this->savedPitchesTimeLimit = savedPitchesTimeLimit;
}
