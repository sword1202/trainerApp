#include "QmlCppBridge.h"
#include <iostream>
#include <QScreen>
#include "../PitchDetection/CppUtils/TimeUtils.h"

using namespace CppUtils;

QmlCppBridge::QmlCppBridge(MainWindow* view) : view(view) {
    connect(view, &MainWindow::resize, view, [=] {
        emit resize();
    });
}

int QmlCppBridge::windowWidth() {
    return view->size().width();
}

int QmlCppBridge::windowHeight() {
    return view->size().height();
}

int QmlCppBridge::dp(qreal dp) {
    return qRound(view->screen()->logicalDotsPerInch() * dp);
}

int QmlCppBridge::dpX(qreal dp) {
    return qRound(view->screen()->logicalDotsPerInchX() * dp);
}

int QmlCppBridge::dpY(qreal dp) {
    return qRound(view->screen()->logicalDotsPerInchY() * dp);
}

qreal QmlCppBridge::now() {
    return TimeUtils::NowInSeconds();
}

QmlPitch QmlCppBridge::pitchWithFrequency(float frequency) {
    return QmlPitch(frequency, -1);
}

QmlPitch QmlCppBridge::pitchFromPerfectFrequencyIndex(int perfectFrequencyIndex) {
    return QmlPitch(perfectFrequencyIndex);
}


QmlPitch QmlCppBridge::whitePitch(int whitePitchIndex, int octaveIndex) {
    return QmlPitch(Pitch::whitePitch(whitePitchIndex, octaveIndex).getPerfectFrequencyIndex());
}
