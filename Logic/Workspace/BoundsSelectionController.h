//
// Created by Semyon Tykhonenko on 8/24/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_BOUNDSSELECTIONCONTROLLER_H
#define VOCALTRAINER_BOUNDSSELECTIONCONTROLLER_H

#include "BoundsSelectionDelegate.h"
#include "MouseEventsReceiver.h"
#include "WorkspaceDrawer.h"

class WorkspaceDrawer;

class BoundsSelectionController {
    BoundsSelectionDelegate* delegate;
    MouseEventsReceiver* mouseEventsReceiver;
    WorkspaceDrawer* workspaceDrawer;
    bool boundsSelectionRunning = false;
    bool boundsSelectionEnabled = false;
    PlaybackBounds selectedBounds;

    void startBoundsSelection();
    void stopBoundsSelection();
public:
    BoundsSelectionController(BoundsSelectionDelegate *delegate, MouseEventsReceiver *mouseEventsReceiver,
            WorkspaceDrawer *workspaceDrawer);

    void setBoundsSelectionEnabled(bool boundsSelectionEnabled);
    void update();
};


#endif //VOCALTRAINER_BOUNDSSELECTIONCONTROLLER_H
