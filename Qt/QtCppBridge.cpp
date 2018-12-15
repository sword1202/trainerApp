#include "QtCppBridge.h"
#include <iostream>
#include <QScreen>
#include "TimeUtils.h"
#include "VxApp.h"

using namespace CppUtils;

QtCppBridge::QtCppBridge(QMainWindow* mainWindow) : _mainWindow(mainWindow) {

}

qreal QtCppBridge::now() {
    return TimeUtils::NowInSeconds();
}

QmlPitch QtCppBridge::pitchWithFrequency(float frequency) {
    return QmlPitch(frequency);
}

QmlPitch QtCppBridge::pitchFromPerfectFrequencyIndex(int perfectFrequencyIndex) {
    return QmlPitch(perfectFrequencyIndex);
}


QmlPitch QtCppBridge::whitePitch(int whitePitchIndex, int octaveIndex) {
    return QmlPitch(Pitch::whitePitchPerfectFrequencyIndex(whitePitchIndex, octaveIndex));
}

QmlWorkspaceZoomController *QtCppBridge::getZoomController() const {
    return static_cast<QmlWorkspaceZoomController*>(VxApp::instance()->getZoomController());
}

QtMvxPlayer *QtCppBridge::getPlayer() const {
    return static_cast<QtMvxPlayer*>(VxApp::instance()->getPlayer());
}

qreal QtCppBridge::getDevicePixelRatio() const {
    return _mainWindow->devicePixelRatio();
}

QMainWindow *QtCppBridge::getMainWindow() const {
    return _mainWindow;
}
