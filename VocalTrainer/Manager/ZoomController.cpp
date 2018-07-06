//
// Created by Semyon Tikhonenko on 7/6/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "ZoomController.h"
#include "Pitch.h"
#include <cmath>

constexpr float BASE_WIDTH = 1374.0;
constexpr float ZOOM_FACTOR = 12.0f * 4; // number of beats in screen of BASE_WIDTH for zoom = 1.0
constexpr float MIN_ZOOM = 1.0;
constexpr float MAX_ZOOM = 2.0;
constexpr float HORIZONTAL_TO_VERTICAL_INTERVAL_WIDTH_RELATION = 2.4117701323665077f;

ZoomController::ZoomController() {
    zoom = MIN_ZOOM;
    firstPitch = Pitch("C2");
}

float ZoomController::getIntervalWidth() const {
    int linesCount = (int)round(ZOOM_FACTOR / zoom);
    int baseIntervalsCount = linesCount + 1;
    return BASE_WIDTH / baseIntervalsCount;
}

float ZoomController::getIntervalHeight() const {
    return getIntervalWidth() / HORIZONTAL_TO_VERTICAL_INTERVAL_WIDTH_RELATION;
}

float ZoomController::getZoom() const {
    return zoom;
}

void ZoomController::setZoom(float zoom) {
    assert(zoom >= MIN_ZOOM && zoom <= MAX_ZOOM);
    this->zoom = zoom;
    zoomChangedListeners.executeAll(zoom);
}

float ZoomController::getMinZoom() const {
    return MIN_ZOOM;
}

float ZoomController::getMaxZoom() const {
    return MAX_ZOOM;
}

Pitch ZoomController::getFirstPitch() const {
    return firstPitch;
}

void ZoomController::setFirstPitch(const Pitch& pitch) {
    assert(pitch.isWhite());
    firstPitch = pitch;
    firstPitchChangedListeners.executeAll(pitch);
}

int ZoomController::addFirstPitchChangedListener(const CppUtils::ListenersSet<Pitch>::Listener &listener) {
    return firstPitchChangedListeners.addListener(listener);
}

void ZoomController::removeFirstPitchChangedListener(int key) {
    firstPitchChangedListeners.removeListener(key);
}

int ZoomController::addZoomChangedListener(const CppUtils::ListenersSet<float>::Listener &listener) {
    return zoomChangedListeners.addListener(listener);
}

void ZoomController::removeZoomChangedListener(int key) {
    zoomChangedListeners.removeListener(key);
}
