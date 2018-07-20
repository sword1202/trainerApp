#ifndef QMLCPPBRIDGE_H
#define QMLCPPBRIDGE_H

#include <QObject>
#include <QSize>
#include <QQuickView>
#include "qmlpitch.h"
#include "mainwindow.h"
#include "qmlzoomcontroller.h"
#include "player.h"
#include "qmlpitchinputreader.h"

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

    Q_PROPERTY(QmlZoomController* zoomController READ
            getZoomController() CONSTANT)
    Q_PROPERTY(Player* player READ getPlayer() CONSTANT)
    Q_PROPERTY(QmlPitchInputReader* pitchInputReader READ getPitchInputReader() CONSTANT)
    Q_PROPERTY(qreal devicePixelRatio READ getDevicePixelRatio() CONSTANT)

    QmlZoomController* getZoomController() const;
    Player* getPlayer() const;
    QmlPitchInputReader* getPitchInputReader() const;
    qreal getDevicePixelRatio() const;

signals:
    void resize();
};

#endif // QMLCPPBRIDGE_H
