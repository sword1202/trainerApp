//
// Created by Semyon Tikhonenko on 10/1/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_PLAYBACKBOUNDSSELECTIONCONTROLLER_H
#define VOCALTRAINER_PLAYBACKBOUNDSSELECTIONCONTROLLER_H

#include "WorkspaceController.h"
#include "MvxPlayer.h"
#include "CppUtils/ListenersSet.h"

class PlaybackBoundsSelectionController {
    WorkspaceController* workspaceController;
    MvxPlayer* player;

    bool boundsSelectionEnabled = false;
    PlaybackBounds selectedBounds;

    CppUtils::ListenersSet<bool> boundsSelectionEnabledListeners;
public:

    PlaybackBoundsSelectionController(WorkspaceController *workspaceController, MvxPlayer *player);

    void setWorkspaceController(WorkspaceController *workspaceController);

    void setBoundsSelectionEnabled(bool boundsSelectionEnabled);

    void onWorkspaceMouseMove(float x);
    void onWorkspaceMouseClick(float x);

    void addBoundsSelectionEnabledChangedListener(const std::function<void(bool)>& func);
};


#endif //VOCALTRAINER_PLAYBACKBOUNDSSELECTIONCONTROLLER_H
