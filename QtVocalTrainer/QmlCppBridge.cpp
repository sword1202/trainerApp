#include "QmlCppBridge.h"
#include <iostream>
#include <QScreen>
#include "../PitchDetection/CppUtils/TimeUtils.h"

using namespace CppUtils;

QmlCppBridge::QmlCppBridge(MainWindow *mainWindow) : mainWindow(mainWindow) {

}

qreal QmlCppBridge::now() {
    return TimeUtils::NowInSeconds();
}

QmlPitch QmlCppBridge::pitchWithFrequency(float frequency) {
    return QmlPitch(frequency);
}

QmlPitch QmlCppBridge::pitchFromPerfectFrequencyIndex(int perfectFrequencyIndex) {
    return QmlPitch(perfectFrequencyIndex);
}


QmlPitch QmlCppBridge::whitePitch(int whitePitchIndex, int octaveIndex) {
    return QmlPitch(Pitch::whitePitch(whitePitchIndex, octaveIndex).getPerfectFrequencyIndex());
}

ZoomController *QmlCppBridge::getZoom() const {
    return ZoomController::instance();
}
