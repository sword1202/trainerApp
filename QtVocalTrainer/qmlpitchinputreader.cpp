#include "qmlpitchinputreader.h"
#include "../PitchDetection/CppUtils/TimeUtils.h"
#include "../PitchDetection/CppUtils/Executors.h"
#include "app.h"

static const int PITCH_DETECTION_BUFFER_SIZE = 1024;
static const int PITCH_SMOOTH_LEVEL = 2;
// minimum pitch detection probability is 0.75. Pitches with lower probability are marked as invalid
static const float PITCH_DETECTION_THRESHOLD = 0.25;

QmlPitchInputReader::QmlPitchInputReader(QObject *parent) : QObject(parent) {
    AudioInputReader* audioInputReader = CreateDefaultAudioInputReader(PITCH_DETECTION_BUFFER_SIZE);
    init(audioInputReader, PITCH_SMOOTH_LEVEL, true);
    setThreshold(PITCH_DETECTION_THRESHOLD);
}

void QmlPitchInputReader::start() {
    PitchInputReaderCollector::start();
}

void QmlPitchInputReader::stop() {
    PitchInputReaderCollector::stop();
}

QmlTimedPitch QmlPitchInputReader::pitchAt(int index) {
    return QmlTimedPitch(getFrequencyAt(index), getTimeAt(index));
}

void QmlPitchInputReader::pitchDetected(float frequency, double time) {
    emit pitchDetected(QmlTimedPitch(frequency, time));
}

void QmlPitchInputReader::setThreshold(float threshold) {
    PitchInputReaderCollector::setThreshold(threshold);
    emit thresholdChanged(threshold);
}

QmlPitchInputReader* QmlPitchInputReader::instance() {
    static QmlPitchInputReader reader;
    return &reader;
}

QmlTimedPitch QmlPitchInputReader::getLastDetectedPitch() {
    return QmlTimedPitch(getLastDetectedFrequency(), getLastDetectedTime());
}
