#include "QmlWorkspaceZoomController.h"
#include "Executors.h"

using namespace CppUtils;

QmlWorkspaceZoomController::QmlWorkspaceZoomController() {
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

void QmlWorkspaceZoomController::onWorkspaceWidgetHeightChanged(float height) {
    WorkspaceZoomController::onWorkspaceWidgetHeightChanged(height);
    Executors::ExecuteOnMainThread([this] {
        emit pageSizeChanged();
    });
}
