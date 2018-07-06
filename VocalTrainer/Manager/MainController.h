//
// Created by Semyon Tikhonenko on 7/3/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_MANAGER_H
#define VOCALTRAINER_MANAGER_H


#include "PitchInputReaderCollector.h"
#include "VxPitchInputReader.h"
#include "MvxPlayer.h"
#include "WorkspaceController.h"
#include "ZoomController.h"
#include "PianoController.h"

class MainController {
    VxPitchInputReader* pitchInputReader;
    MvxPlayer* mvxPlayer;
    ZoomController* zoomController;
    std::atomic<WorkspaceController*> workspaceController;
    PianoController* pianoController = nullptr;
    std::function<void()> onPianoUpdateRequested;

    void updateZoom();
    void updateWorkspaceFirstPitch();
public:
    MainController();

    VxPitchInputReader *getPitchInputReader() const;
    MvxPlayer *getMvxPlayer() const;
    ZoomController *getZoomController() const;

    // Should be executed on a render thread, the same thread as workspace->draw is executed
    void setWorkspaceController(WorkspaceController* workspaceController);
    void updateWorkspaceIsPlayingChanged(bool playing);

    void setPianoController(PianoController *pianoController, const std::function<void()>& onUpdateRequested);

    static MainController* instance();

    virtual ~MainController();
};


#endif //VOCALTRAINER_MANAGER_H
