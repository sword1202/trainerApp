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
}
