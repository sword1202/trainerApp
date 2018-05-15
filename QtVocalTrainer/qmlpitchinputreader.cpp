#include "qmlpitchinputreader.h"

static const int PITCH_DETECTION_BUFFER_SIZE = 1200;
static const int PITCH_SMOOTH_LEVEL = 4;

QmlPitchInputReader::QmlPitchInputReader(QObject *parent) : QObject(parent) {
    audioInputReader = CreateDefaultAudioInputReader(PITCH_DETECTION_BUFFER_SIZE);
    pitchInputReader = new PitchInputReader(audioInputReader, PITCH_SMOOTH_LEVEL);
    pitchInputReader->setExecuteCallBackOnInvalidPitches(true);
    pitchInputReader->setCallback([=](const Pitch& pitch) {
        emit pitchDetected(QmlPitch(pitch));
    });
    pitchInputReader->start();
}

QmlPitchInputReader::~QmlPitchInputReader() {
    if (pitchInputReader) {
        delete pitchInputReader;
    }
}
