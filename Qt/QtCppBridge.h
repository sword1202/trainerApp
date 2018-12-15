#ifndef QMLCPPBRIDGE_H
#define QMLCPPBRIDGE_H

#include <QObject>
#include <QSize>
#include <QQuickView>
#include "QmlPitch.h"
#include "QmlWorkspaceZoomController.h"
#include "QtMvxPlayer.h"
#include <QMainWindow>

class QtCppBridge : public QObject
{
    Q_OBJECT

    QMainWindow* _mainWindow;

public:
    QtCppBridge(QMainWindow* mainWindow);

    Q_INVOKABLE qreal now();

    Q_INVOKABLE QmlPitch pitchWithFrequency(float frequency);
    Q_INVOKABLE QmlPitch pitchFromPerfectFrequencyIndex(int perfectFrequencyIndex);
    Q_INVOKABLE QmlPitch whitePitch(int whitePitchIndex, int octaveIndex);

    Q_PROPERTY(QmlWorkspaceZoomController* zoomController READ
            getZoomController() CONSTANT)
    Q_PROPERTY(QtMvxPlayer* player READ getPlayer() CONSTANT)
    Q_PROPERTY(qreal devicePixelRatio READ getDevicePixelRatio() CONSTANT)
    Q_PROPERTY(QMainWindow* mainWindow READ getMainWindow() CONSTANT)

    QmlWorkspaceZoomController* getZoomController() const;
    QtMvxPlayer* getPlayer() const;
    qreal getDevicePixelRatio() const;
    QMainWindow* getMainWindow() const;

signals:
    void resize();
    void workspaceHorizontalOffsetChanged();
};

#endif // QMLCPPBRIDGE_H
