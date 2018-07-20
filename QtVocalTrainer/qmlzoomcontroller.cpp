#include "qmlzoomcontroller.h"

using namespace CppUtils;

QmlZoomController::QmlZoomController() {
    addZoomChangedListener([=] (float zoom) {
        emit zoomChanged();
        return DONT_DELETE_LISTENER;
    });

    addFirstPitchChangedListener([=] (Pitch) {
        emit firstPitchChanged();
        return DONT_DELETE_LISTENER;
    });
}
