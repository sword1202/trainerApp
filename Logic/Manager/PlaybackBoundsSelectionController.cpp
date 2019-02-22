//
// Created by Semyon Tikhonenko on 10/1/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "PlaybackBoundsSelectionController.h"
#include "Executors.h"

using namespace CppUtils;

PlaybackBoundsSelectionController::PlaybackBoundsSelectionController(
        WorkspaceController *workspaceController, MvxPlayer *player)
        : workspaceController(workspaceController),
        player(player),
        boundsSelectionEnabledListeners() {

}

void PlaybackBoundsSelectionController::setWorkspaceController(WorkspaceController *workspaceController) {
    PlaybackBoundsSelectionController::workspaceController = workspaceController;
}

void PlaybackBoundsSelectionController::setBoundsSelectionEnabled(bool boundsSelectionEnabled) {
    Executors::ExecuteOnMainThread([=] {
        this->boundsSelectionEnabled = boundsSelectionEnabled;
        boundsSelectionEnabledListeners.executeAll(boundsSelectionEnabled);

        if (!boundsSelectionEnabled) {
            player->setBounds(PlaybackBounds());
        } else {
            selectedBounds.setStartSeek(workspaceController->getWorkspaceSeek());
        }
    });
}

void PlaybackBoundsSelectionController::onWorkspaceMouseMove(float x) {
    if (!boundsSelectionEnabled) {
        return;
    }

    float seek = workspaceController->getSeekFromXPositionOnWorkspace(x);
    float minimumSeek = player->getTactDuration();
    seek = std::max(seek, minimumSeek);
    selectedBounds.setEndSeek(seek);
    workspaceController->setPlaybackBounds(selectedBounds);
    workspaceController->update();
}

void PlaybackBoundsSelectionController::onWorkspaceMouseClick(float x) {
    if (!boundsSelectionEnabled) {
        return;
    }

    boundsSelectionEnabled = false;
    player->setBounds(selectedBounds);
}

void PlaybackBoundsSelectionController::addBoundsSelectionEnabledChangedListener(
        const std::function<void(bool)> &func) {
    Executors::ExecuteOnMainThread([=] {
        boundsSelectionEnabledListeners.addListener(func);
    });
}
