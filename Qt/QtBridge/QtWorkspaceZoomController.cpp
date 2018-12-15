#include "QtWorkspaceZoomController.h"
#include "Executors.h"

using namespace CppUtils;

QtWorkspaceZoomController::QtWorkspaceZoomController() {
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

void QtWorkspaceZoomController::onWorkspaceWidgetHeightChanged(float height) {
    WorkspaceZoomController::onWorkspaceWidgetHeightChanged(height);
    Executors::ExecuteOnMainThread([this] {
        emit pageSizeChanged();
    });
}
