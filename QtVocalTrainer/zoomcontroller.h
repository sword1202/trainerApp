#ifndef ZOOM_H
#define ZOOM_H

#include <QObject>
#include "qmlpitch.h"

class ZoomController : public QObject
{
    Q_OBJECT
    qreal zoom;
    int firstPitchPerfectFrequencyIndex;
public:
    static ZoomController* instance();

    Q_PROPERTY(qreal zoom READ getZoom() WRITE setZoom() NOTIFY zoomChanged())
    Q_PROPERTY(qreal intervalWidth READ getIntervalWidth() NOTIFY intervalWidthChanged())
    Q_PROPERTY(qreal intervalHeight READ getIntervalHeight() NOTIFY intervalHeightChanged())
    Q_PROPERTY(qreal minZoom READ getMinZoom() CONSTANT)
    Q_PROPERTY(qreal maxZoom READ getMaxZoom() CONSTANT)
    Q_PROPERTY(qreal firstPitchPerfectFrequencyIndex READ getFirstPitchPerfectFrequencyIndex() WRITE
            setFirstPitchPerfectFrequencyIndex() NOTIFY firstPitchPerfectFrequencyIndexChanged())
    Q_PROPERTY(Pitch firstPitch READ getFirstPitch() NOTIFY firstPitchPerfectFrequencyIndexChanged())

    ZoomController();
    qreal getIntervalWidth() const;
    qreal getIntervalHeight() const;

    qreal getZoom() const;
    void setZoom(qreal zoom);

    qreal getMinZoom() const;
    qreal getMaxZoom() const;

    int getFirstPitchPerfectFrequencyIndex() const;
    void setFirstPitchPerfectFrequencyIndex(int firstPitchPerfectFrequencyIndex);
    QmlPitch getFirstPitch() const;

signals:
    void zoomChanged();
    void intervalWidthChanged();
    void intervalHeightChanged();
    void firstPitchPerfectFrequencyIndexChanged();
};

#endif // ZOOM_H
