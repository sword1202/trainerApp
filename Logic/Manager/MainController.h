//
// Created by Semyon Tikhonenko on 7/3/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_MANAGER_H
#define VOCALTRAINER_MANAGER_H


#include "AudioInputPitchesRecorder.h"
#include "AudioInputManager.h"
#include "MvxPlayer.h"
#include "WorkspaceController.h"
#include "PlaybackBounds.h"
#include "Controllers/PlaybackBoundsSelectionController.h"
#include "CallbacksQueue.h"

class MainController {
    AudioInputManager* audioInputManager = nullptr;
    MvxPlayer* mvxPlayer = nullptr;
    WorkspaceController* workspaceController = nullptr;
    PlaybackBoundsSelectionController* playbackBoundsSelectionController = nullptr;
    CppUtils::CallbacksQueue workspaceControllerReadyCallbacksQueue;

    void onStopPlaybackRequested();
    void updateSeek(double seek);

    void generateRecording(MvxFile* out);
public:

    void init(AudioInputManager *pitchInputReader, MvxPlayer *mvxPlayer);

    AudioInputManager *getAudioInputManager() const;

    MvxPlayer *getPlayer() const;
    PlaybackBoundsSelectionController* getPlaybackBoundsSelectionController() const;

    // Should be executed on a render thread, the same thread as workspace->draw is executed
    void setWorkspaceController(WorkspaceController* workspaceController);

    void getWorkspaceController(const std::function<void(WorkspaceController*)>& callback);

    void saveRecordingIntoFile(const char* filePath);

    static MainController* instance();
    static void initInstance(MainController* inst);

    virtual ~MainController();
};


#endif //VOCALTRAINER_MANAGER_H
