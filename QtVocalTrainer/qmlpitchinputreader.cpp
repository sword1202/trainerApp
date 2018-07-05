#include "qmlpitchinputreader.h"

QmlPitchInputReader::QmlPitchInputReader(QObject *parent) : QObject(parent) {

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
