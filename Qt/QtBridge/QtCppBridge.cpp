#include "QtCppBridge.h"
#include <iostream>
#include <QScreen>
#include "TimeUtils.h"
#include "App/VxApp.h"

using namespace CppUtils;

QtCppBridge::QtCppBridge(QMainWindow* mainWindow) : _mainWindow(mainWindow) {

}

qreal QtCppBridge::now() {
    return TimeUtils::NowInSeconds();
}

QtPitch QtCppBridge::pitchWithFrequency(float frequency) {
    return QtPitch(frequency);
}

QtPitch QtCppBridge::pitchFromPerfectFrequencyIndex(int perfectFrequencyIndex) {
    return QtPitch(perfectFrequencyIndex);
}


QtPitch QtCppBridge::whitePitch(int whitePitchIndex, int octaveIndex) {
    return QtPitch(Pitch::whitePitchPerfectFrequencyIndex(whitePitchIndex, octaveIndex));
}

QtWorkspaceZoomController *QtCppBridge::getZoomController() const {
    return static_cast<QtWorkspaceZoomController*>(VxApp::instance()->getZoomController());
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
