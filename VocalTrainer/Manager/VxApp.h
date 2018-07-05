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

class VxApp {
    static VxApp* _instance;
    VxPitchInputReader* pitchInputReader;
    MvxPlayer* mvxPlayer;
    std::atomic<WorkspaceController*> workspaceController;

public:
    VxApp(VxPitchInputReader* pitchInputReader, MvxPlayer* mvxPlayer);

    VxPitchInputReader *getPitchInputReader() const;
    MvxPlayer *getMvxPlayer() const;

    // Should be executed on a render thread, the same thread as workspace->draw is executed
    void setWorkspaceController(WorkspaceController* workspaceController);

    void updateWorkspaceIsPlayingChanged(bool playing);

    static void initInstance(VxApp* app);
    static VxApp* instance();

    virtual ~VxApp();
};


#endif //VOCALTRAINER_MANAGER_H
