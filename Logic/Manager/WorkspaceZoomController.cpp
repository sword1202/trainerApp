//
// Created by Semyon Tikhonenko on 7/6/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "WorkspaceZoomController.h"
#include "Pitch.h"
#include <cmath>
#include <cassert>
#include "WorkspaceDrawer.h"
#include "Executors.h"


constexpr float BASE_WIDTH = 1374.0;
constexpr float ZOOM_FACTOR = 12.0f * 4; // number of beats in screen of BASE_WIDTH for zoom = 1.0
constexpr float MIN_ZOOM = 1.0;
constexpr float MAX_ZOOM = 2.0;
constexpr float HORIZONTAL_TO_VERTICAL_INTERVAL_WIDTH_RELATION = 2.4117701323665077f;

WorkspaceZoomController::WorkspaceZoomController() {
    zoom = MIN_ZOOM;
    verticalScrollPosition = 0;
    workspaceGridHeight = 0;
    firstPitchIndex = Pitch("C2").getPerfectFrequencyIndex();
    lastPitchIndex = Pitch("B6").getPerfectFrequencyIndex();
}

float WorkspaceZoomController::getIntervalWidth() const {
    int linesCount = (int)round(ZOOM_FACTOR / zoom);
    int baseIntervalsCount = linesCount + 1;
    return BASE_WIDTH / baseIntervalsCount;
}

float WorkspaceZoomController::getIntervalHeight() const {
    return getIntervalWidth() / HORIZONTAL_TO_VERTICAL_INTERVAL_WIDTH_RELATION;
}

float WorkspaceZoomController::getZoom() const {
    return zoom;
}

void WorkspaceZoomController::setZoom(float zoom) {
    assert(zoom >= MIN_ZOOM && zoom <= MAX_ZOOM);
    this->zoom = zoom;
    zoomChangedListeners.executeAll(zoom);
    summarizedWorkspaceGridHeightChangedListeners.executeAll();
}

float WorkspaceZoomController::getMinZoom() const {
    return MIN_ZOOM;
}

float WorkspaceZoomController::getMaxZoom() const {
    return MAX_ZOOM;
}

Pitch WorkspaceZoomController::getFirstPitch() const {
    return Pitch::fromPerfectFrequencyIndex(firstPitchIndex);
}

void WorkspaceZoomController::setFirstPitch(const Pitch& pitch) {
    assert(pitch.isWhite());
    firstPitchIndex = pitch.getPerfectFrequencyIndex();

    firstPitchChangedListeners.executeAll(pitch);
    summarizedWorkspaceGridHeightChangedListeners.executeAll();
}

const Pitch &WorkspaceZoomController::getLastPitch() const {
    return Pitch::fromPerfectFrequencyIndex(lastPitchIndex);
}

void WorkspaceZoomController::setLastPitch(const Pitch &lastPitch) {
    this->lastPitchIndex = lastPitch.getPerfectFrequencyIndex();
    lastPitchChangedListeners.executeAll(lastPitch);
    summarizedWorkspaceGridHeightChangedListeners.executeAll();
}

float WorkspaceZoomController::getSummarizedWorkspaceGridHeight() const {
    return (lastPitchIndex - firstPitchIndex + 1) * getIntervalHeight();
}

float WorkspaceZoomController::getVerticalScrollPosition() const {
    return verticalScrollPosition;
}

void WorkspaceZoomController::setVerticalScrollPosition(float verticalScrollPosition) {
    assert(verticalScrollPosition >= 0 && verticalScrollPosition <= 1);
    this->verticalScrollPosition = verticalScrollPosition;
    verticalScrollPositionChangedListeners.executeAll(verticalScrollPosition);
}

float WorkspaceZoomController::getPageSize() const {
    const float result = workspaceGridHeight / getSummarizedWorkspaceGridHeight();
    return result > 1.0f ? 1.0f : result;
}

void WorkspaceZoomController::onWorkspaceWidgetHeightChanged(float height) {
    workspaceGridHeight = WorkspaceDrawer::getVisibleGridHeight(height);
}
