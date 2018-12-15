#ifndef QMLCPPBRIDGE_H
#define QMLCPPBRIDGE_H

#include <QObject>
#include <QSize>
#include <QQuickView>
#include "QtPitch.h"
#include "QtWorkspaceZoomController.h"
#include "QtMvxPlayer.h"
#include <QWidget>

class QtCppBridge : public QObject
{
    Q_OBJECT

    QWidget* _widget;

public:
    explicit QtCppBridge(QWidget* widget);

    Q_INVOKABLE qreal now();

    Q_INVOKABLE QtPitch pitchWithFrequency(float frequency);
    Q_INVOKABLE QtPitch pitchFromPerfectFrequencyIndex(int perfectFrequencyIndex);
    Q_INVOKABLE QtPitch whitePitch(int whitePitchIndex, int octaveIndex);

    Q_PROPERTY(QtWorkspaceZoomController* zoomController READ
            getZoomController() CONSTANT)
    Q_PROPERTY(QtMvxPlayer* player READ getPlayer() CONSTANT)
    Q_PROPERTY(qreal devicePixelRatio READ getDevicePixelRatio() CONSTANT)
    Q_PROPERTY(QWidget* widget READ getWidget() CONSTANT)

    QtWorkspaceZoomController* getZoomController() const;
    QtMvxPlayer* getPlayer() const;
    qreal getDevicePixelRatio() const;
    QWidget* getWidget() const;

signals:
    void resize();
    void workspaceHorizontalOffsetChanged();
};

#endif // QMLCPPBRIDGE_H
