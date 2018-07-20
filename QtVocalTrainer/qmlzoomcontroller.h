#ifndef ZOOM_CONTROLLER_H
#define ZOOM_CONTROLLER_H

#include <QObject>
#include "qmlpitch.h"
#include "ZoomController.h"

class QmlZoomController : public QObject, public ZoomController
{
    Q_OBJECT

public:
    Q_PROPERTY(qreal zoom READ getZoom() WRITE setZoom() NOTIFY zoomChanged())
    Q_PROPERTY(qreal intervalWidth READ getIntervalWidth() NOTIFY zoomChanged())
    Q_PROPERTY(qreal intervalHeight READ getIntervalHeight() NOTIFY zoomChanged())
    Q_PROPERTY(qreal minZoom READ getMinZoom() CONSTANT)
    Q_PROPERTY(qreal maxZoom READ getMaxZoom() CONSTANT)
    Q_PROPERTY(QmlPitch firstPitch READ getFirstPitch() NOTIFY firstPitchChanged())

    QmlZoomController();
signals:
    void zoomChanged();
    void firstPitchChanged();
};

#endif // ZOOM_CONTROLLER_H
