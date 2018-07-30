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
    lastPitch = Pitch("B6");
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
    summarizedWorkspaceGridHeightChangedListeners.executeAll();
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
    summarizedWorkspaceGridHeightChangedListeners.executeAll();
}

const Pitch &ZoomController::getLastPitch() const {
    return lastPitch;
}

void ZoomController::setLastPitch(const Pitch &lastPitch) {
    this->lastPitch = lastPitch;
    lastPitchChangedListeners.executeAll(lastPitch);
    summarizedWorkspaceGridHeightChangedListeners.executeAll();
}

float ZoomController::getSummarizedWorkspaceGridHeight() const {
    return (lastPitch.getPerfectFrequencyIndex() - firstPitch.getPerfectFrequencyIndex() + 1) * getIntervalHeight();
}

float ZoomController::getVerticalScrollPosition() const {
    return verticalScrollPosition;
}

void ZoomController::setVerticalScrollPosition(float verticalScrollPosition) {
    assert(verticalScrollPosition >= 0 && verticalScrollPosition <= 1);
    this->verticalScrollPosition = verticalScrollPosition;
    verticalScrollPositionChangedListeners.executeAll(verticalScrollPosition);
}

float ZoomController::getWorkspaceGridHeight() const {
    return workspaceGridHeight;
}

void ZoomController::setWorkspaceGridHeight(float pageSize) {
    this->workspaceGridHeight = pageSize;
}

float ZoomController::getPageSize() const {
    const float result = workspaceGridHeight / getSummarizedWorkspaceGridHeight();
    return result > 1.0f ? 1.0f : result;
}
