#include "QmlCppBridge.h"
#include <iostream>
#include <QScreen>
#include "../PitchDetection/CppUtils/TimeUtils.h"
#include "app.h"

using namespace CppUtils;

QmlCppBridge::QmlCppBridge(MainWindow *mainWindow) : mainWindow(mainWindow) {
    MainController::instance()->addWorkspaceHorizontalOffsetChangedListener([=] (float value) {
        emit workspaceHorizontalOffsetChanged();
        return DONT_DELETE_LISTENER;
    });
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
    return mainWindow->devicePixelRatio();
}

QmlPitchInputReader* QmlCppBridge::getPitchInputReader() const {
    return QmlPitchInputReader::instance();
}

qreal QmlCppBridge::getWorkspaceHorizontalOffset() const {
    return MainController::instance()->getWorkspaceHorizontalOffset();
}
