#ifndef QMLCPPBRIDGE_H
#define QMLCPPBRIDGE_H

#include <QObject>
#include <QSize>
#include <QQuickView>
#include "qmlpitch.h"

class QmlCppBridge : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE qreal now();

    Q_INVOKABLE QmlPitch pitchWithFrequency(float frequency);
    Q_INVOKABLE QmlPitch pitchFromPerfectFrequencyIndex(int perfectFrequencyIndex);
    Q_INVOKABLE QmlPitch whitePitch(int whitePitchIndex, int octaveIndex);

signals:
    void resize();
};

#endif // QMLCPPBRIDGE_H
