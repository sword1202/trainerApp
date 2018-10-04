#include "QmlZoomController.h"

using namespace CppUtils;

QmlZoomController::QmlZoomController() {
    zoomChangedListeners.addListener([=] (float zoom) {
        emit zoomChanged();
    });

    firstPitchChangedListeners.addListener([=] (Pitch) {
        emit firstPitchChanged();
    });

    summarizedWorkspaceGridHeightChangedListeners.addListener([=] {
        emit pageSizeChanged();
    });

    verticalScrollPositionChangedListeners.addListener([=] (float) {
        emit verticalScrollPositionChanged();
    });
}

void QmlZoomController::setWorkspaceGridHeight(float pageSize) {
    ZoomController::setWorkspaceGridHeight(pageSize);
    emit pageSizeChanged();
}
