#ifndef ZOOM_CONTROLLER_H
#define ZOOM_CONTROLLER_H

#include <QObject>
#include "QmlPitch.h"
#include "WorkspaceZoomController.h"

class QmlWorkspaceZoomController : public QObject, public WorkspaceZoomController
{
    Q_OBJECT

public:
    Q_PROPERTY(qreal zoom READ getZoom() WRITE setZoom() NOTIFY zoomChanged())
    Q_PROPERTY(qreal intervalWidth READ getIntervalWidth() NOTIFY zoomChanged())
    Q_PROPERTY(qreal intervalHeight READ getIntervalHeight() NOTIFY zoomChanged())
    Q_PROPERTY(qreal minZoom READ getMinZoom() CONSTANT)
    Q_PROPERTY(qreal maxZoom READ getMaxZoom() CONSTANT)
    Q_PROPERTY(qreal pageSize READ getPageSize() NOTIFY pageSizeChanged())
    Q_PROPERTY(qreal verticalScrollPosition READ getVerticalScrollPosition() WRITE
            setVerticalScrollPosition() NOTIFY verticalScrollPositionChanged())
    Q_PROPERTY(QmlPitch firstPitch READ getFirstPitch() NOTIFY firstPitchChanged())

    void onWorkspaceWidgetHeightChanged(float height) override;

    QmlWorkspaceZoomController();
signals:
    void zoomChanged();
    void firstPitchChanged();
    void pageSizeChanged();
    void verticalScrollPositionChanged();
};

#endif // ZOOM_CONTROLLER_H
