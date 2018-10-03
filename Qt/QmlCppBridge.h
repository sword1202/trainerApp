#ifndef QMLCPPBRIDGE_H
#define QMLCPPBRIDGE_H

#include <QObject>
#include <QSize>
#include <QQuickView>
#include "qmlpitch.h"
#include "qmlzoomcontroller.h"
#include "player.h"
#include <QMainWindow>

class QmlCppBridge : public QObject
{
    Q_OBJECT

    QMainWindow* _mainWindow;

public:
    QmlCppBridge(QMainWindow* mainWindow);

    Q_INVOKABLE qreal now();

    Q_INVOKABLE QmlPitch pitchWithFrequency(float frequency);
    Q_INVOKABLE QmlPitch pitchFromPerfectFrequencyIndex(int perfectFrequencyIndex);
    Q_INVOKABLE QmlPitch whitePitch(int whitePitchIndex, int octaveIndex);

    Q_PROPERTY(QmlZoomController* zoomController READ
            getZoomController() CONSTANT)
    Q_PROPERTY(Player* player READ getPlayer() CONSTANT)
    Q_PROPERTY(qreal devicePixelRatio READ getDevicePixelRatio() CONSTANT)
    Q_PROPERTY(QMainWindow* mainWindow READ getMainWindow() CONSTANT)

    QmlZoomController* getZoomController() const;
    Player* getPlayer() const;
    qreal getDevicePixelRatio() const;
    QMainWindow* getMainWindow() const;

signals:
    void resize();
    void workspaceHorizontalOffsetChanged();
};

#endif // QMLCPPBRIDGE_H
