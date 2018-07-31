#include "qmlzoomcontroller.h"

using namespace CppUtils;

QmlZoomController::QmlZoomController() {
    zoomChangedListeners.addListener([=] (float zoom) {
        emit zoomChanged();
        return DONT_DELETE_LISTENER;
    });

    firstPitchChangedListeners.addListener([=] (Pitch) {
        emit firstPitchChanged();
        return DONT_DELETE_LISTENER;
    });

    summarizedWorkspaceGridHeightChangedListeners.addListener([=] {
        emit pageSizeChanged();
        return DONT_DELETE_LISTENER;
    });

    verticalScrollPositionChangedListeners.addListener([=] (float) {
        emit verticalScrollPositionChanged();
        return DONT_DELETE_LISTENER;
    });
}

void QmlZoomController::setWorkspaceGridHeight(float pageSize) {
    ZoomController::setWorkspaceGridHeight(pageSize);
    emit pageSizeChanged();
}
