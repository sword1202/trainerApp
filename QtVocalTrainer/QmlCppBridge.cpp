#include "QmlCppBridge.h"
#include <iostream>
#include <QScreen>
#include "../PitchDetection/CppUtils/TimeUtils.h"
#include "app.h"

using namespace CppUtils;

QmlCppBridge::QmlCppBridge(QMainWindow* mainWindow) : _mainWindow(mainWindow) {

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
    return QmlPitch(Pitch::whitePitchPerfectFrequencyIndex(whitePitchIndex, octaveIndex));
}

QmlZoomController *QmlCppBridge::getZoomController() const {
    return static_cast<QmlZoomController*>(VxApp::instance()->getZoomController());
}

Player *QmlCppBridge::getPlayer() const {
    return static_cast<Player*>(VxApp::instance()->getPlayer());
}

qreal QmlCppBridge::getDevicePixelRatio() const {
    return _mainWindow->devicePixelRatio();
}

QMainWindow *QmlCppBridge::getMainWindow() const {
    return _mainWindow;
}
