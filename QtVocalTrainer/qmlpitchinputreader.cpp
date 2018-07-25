#include "qmlpitchinputreader.h"

using namespace CppUtils;

QmlPitchInputReader::QmlPitchInputReader(QObject *parent) : QObject(parent) {
    addPitchDetectedListener([=] (const Pitch& pitch, double time) {
        emit pitchDetected(QmlTimedPitch(pitch, time));
        return DONT_DELETE_LISTENER;
    });
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

void QmlPitchInputReader::setThreshold(float threshold) {
    PitchInputReaderCollector::setThreshold(threshold);
    emit thresholdChanged(threshold);
}

QmlTimedPitch QmlPitchInputReader::getLastDetectedPitch() {
    return QmlTimedPitch(getLastDetectedFrequency(), getLastDetectedTime());
}
