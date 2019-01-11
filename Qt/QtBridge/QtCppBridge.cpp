#include "QtCppBridge.h"
#include <iostream>
#include <QScreen>
#include "TimeUtils.h"
#include "App/VxApp.h"

using namespace CppUtils;

QtCppBridge::QtCppBridge(QWidget* widget) : _widget(widget) {

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

QtMvxPlayer *QtCppBridge::getPlayer() const {
    return static_cast<QtMvxPlayer*>(VxApp::instance()->getPlayer());
}

qreal QtCppBridge::getDevicePixelRatio() const {
    return _widget->devicePixelRatio();
}

QWidget *QtCppBridge::getWidget() const {
    return _widget;
}
