//
// Created by Semyon Tykhonenko on 8/24/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#include "BoundsSelectionController.h"

void BoundsSelectionController::startBoundsSelection() {
    if (boundsSelectionRunning) {
        return;
    }

    boundsSelectionRunning = true;
    selectedBounds.setStartSeek(workspaceDrawer->getWorkspaceSeek());
}

void BoundsSelectionController::stopBoundsSelection() {
    if (!boundsSelectionRunning) {
        return;
    }

    boundsSelectionRunning = false;
    delegate->onPlaybackBoundsChangedByUserEvent(selectedBounds);
}

void BoundsSelectionController::setBoundsSelectionEnabled(bool boundsSelectionEnabled) {
    this->boundsSelectionEnabled = boundsSelectionEnabled;
    if (!boundsSelectionEnabled) {
        delegate->onPlaybackBoundsChangedByUserEvent(PlaybackBounds());
    } else {
        startBoundsSelection();
    }
}

void BoundsSelectionController::update() {
    if (boundsSelectionRunning) {
        float x = mouseEventsReceiver->getMousePosition().x;
        float seek = workspaceDrawer->getSeekFromXPositionOnWorkspace(x);
        float minimumBoundsDuration = float(workspaceDrawer->getBeatDuration() * WorkspaceDrawer::BEATS_IN_TACT);
        seek = std::max(seek, minimumBoundsDuration);
        selectedBounds.setEndSeek(seek);
        workspaceDrawer->setPlaybackBounds(selectedBounds);

        if (mouseEventsReceiver->isLeftMouseDown()) {
            stopBoundsSelection();
        }
    }
}

BoundsSelectionController::BoundsSelectionController(BoundsSelectionDelegate *delegate,
        MouseEventsReceiver *mouseEventsReceiver,
        WorkspaceDrawer *workspaceDrawer)
        : delegate(delegate), mouseEventsReceiver(mouseEventsReceiver), workspaceDrawer(workspaceDrawer) {
}
