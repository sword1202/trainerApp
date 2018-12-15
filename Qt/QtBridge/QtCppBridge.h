#ifndef QMLCPPBRIDGE_H
#define QMLCPPBRIDGE_H

#include <QObject>
#include <QSize>
#include <QQuickView>
#include "QtPitch.h"
#include "QtWorkspaceZoomController.h"
#include "QtMvxPlayer.h"
#include <QMainWindow>

class QtCppBridge : public QObject
{
    Q_OBJECT

    QMainWindow* _mainWindow;

public:
    QtCppBridge(QMainWindow* mainWindow);

    Q_INVOKABLE qreal now();

    Q_INVOKABLE QtPitch pitchWithFrequency(float frequency);
    Q_INVOKABLE QtPitch pitchFromPerfectFrequencyIndex(int perfectFrequencyIndex);
    Q_INVOKABLE QtPitch whitePitch(int whitePitchIndex, int octaveIndex);

    Q_PROPERTY(QtWorkspaceZoomController* zoomController READ
            getZoomController() CONSTANT)
    Q_PROPERTY(QtMvxPlayer* player READ getPlayer() CONSTANT)
    Q_PROPERTY(qreal devicePixelRatio READ getDevicePixelRatio() CONSTANT)
    Q_PROPERTY(QMainWindow* mainWindow READ getMainWindow() CONSTANT)

    QtWorkspaceZoomController* getZoomController() const;
    QtMvxPlayer* getPlayer() const;
    qreal getDevicePixelRatio() const;
    QMainWindow* getMainWindow() const;

signals:
    void resize();
    void workspaceHorizontalOffsetChanged();
};

#endif // QMLCPPBRIDGE_H
