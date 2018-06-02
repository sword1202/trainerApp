#ifndef ZOOM_H
#define ZOOM_H

#include <QObject>

class ZoomController : public QObject
{
    Q_OBJECT
    qreal zoom;
public:
    static ZoomController* instance();

    Q_PROPERTY(qreal zoom READ getZoom() WRITE setZoom() NOTIFY zoomChanged())
    Q_PROPERTY(qreal intervalWidth READ getIntervalWidth() NOTIFY intervalWidthChanged())
    Q_PROPERTY(qreal intervalHeight READ getIntervalHeight() NOTIFY intervalHeightChanged())
    Q_PROPERTY(qreal minZoom READ getMinZoom() CONSTANT)
    Q_PROPERTY(qreal maxZoom READ getMaxZoom() CONSTANT)

    ZoomController();
    qreal getIntervalWidth() const;
    qreal getIntervalHeight() const;

    qreal getZoom() const;
    void setZoom(qreal zoom);

    qreal getMinZoom() const;
    qreal getMaxZoom() const;

signals:
    void zoomChanged();
    void intervalWidthChanged();
    void intervalHeightChanged();
};

#endif // ZOOM_H
