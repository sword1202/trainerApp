#include "zoomcontroller.h"

constexpr qreal BASE_WIDTH = 1374.0;
constexpr qreal ZOOM_FACTOR = 12.0 * 4; // number of beats in screen of BASE_WIDTH for zoom = 1.0
constexpr qreal MIN_ZOOM = 1.0;
constexpr qreal MAX_ZOOM = 2.0;
constexpr qreal HORIZONTAL_TO_VERTICAL_INTERVAL_WIDTH_RELATION = 2.4117701323665077;

ZoomController::ZoomController() {
    zoom = MIN_ZOOM;
}

qreal ZoomController::getIntervalWidth() const {
    int linesCount = qRound(ZOOM_FACTOR / zoom);
    int baseIntervalsCount = linesCount + 1;
    return BASE_WIDTH / baseIntervalsCount;
}

qreal ZoomController::getIntervalHeight() const {
    return getIntervalWidth() / HORIZONTAL_TO_VERTICAL_INTERVAL_WIDTH_RELATION;
}

qreal ZoomController::getZoom() const {
    return zoom;
}

void ZoomController::setZoom(qreal zoom) {
    ZoomController::zoom = zoom;
    emit zoomChanged();
    emit intervalWidthChanged();
    emit intervalHeightChanged();
}

qreal ZoomController::getMinZoom() const {
    return MIN_ZOOM;
}

qreal ZoomController::getMaxZoom() const {
    return MAX_ZOOM;
}

ZoomController *ZoomController::instance() {
    static ZoomController* inst = new ZoomController();
    return inst;
}
