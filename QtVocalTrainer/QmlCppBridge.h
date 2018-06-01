#ifndef QMLCPPBRIDGE_H
#define QMLCPPBRIDGE_H

#include <QObject>
#include <QSize>
#include <QQuickView>
#include "qmlpitch.h"
#include "mainwindow.h"

class QmlCppBridge : public QObject
{
    Q_OBJECT

    MainWindow* mainWindow;
public:
    QmlCppBridge(MainWindow* mainWindow);

    Q_INVOKABLE qreal now();

    Q_INVOKABLE QmlPitch pitchWithFrequency(float frequency);
    Q_INVOKABLE QmlPitch pitchFromPerfectFrequencyIndex(int perfectFrequencyIndex);
    Q_INVOKABLE QmlPitch whitePitch(int whitePitchIndex, int octaveIndex);

    Q_INVOKABLE void updateHeader();

signals:
    void resize();
};

#endif // QMLCPPBRIDGE_H
