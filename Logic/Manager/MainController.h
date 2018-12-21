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
#include "WorkspaceZoomController.h"
#include "PlaybackBounds.h"
#include "PlaybackBoundsSelectionController.h"

class MainController {
    AudioInputManager* audioInputManager = nullptr;
    MvxPlayer* mvxPlayer = nullptr;
    WorkspaceZoomController* zoomController = nullptr;
    WorkspaceController* workspaceController = nullptr;
    PlaybackBoundsSelectionController* playbackBoundsSelectionController = nullptr;

    void updateZoom();
    void updateWorkspaceFirstPitch();

    void onStopPlaybackRequested();
    void updateSeek(double seek);

    void generateRecording(MvxFile* out);
public:
    typedef typename CppUtils::ListenersSet<float>::Listener WorkspaceHorizontalOffsetChangedListener;

    void init(AudioInputManager *pitchInputReader, MvxPlayer *mvxPlayer, WorkspaceZoomController *zoomController);

    AudioInputManager *getAudioInputManager() const;

    MvxPlayer *getPlayer() const;
    WorkspaceZoomController *getZoomController() const;
    PlaybackBoundsSelectionController* getPlaybackBoundsSelectionController() const;

    // Should be executed on a render thread, the same thread as workspace->draw is executed
    void setWorkspaceController(WorkspaceController* workspaceController);

    void saveRecordingIntoFile(const char* filePath);

    static MainController* instance();
    static void initInstance(MainController* inst);

    virtual ~MainController();
};


#endif //VOCALTRAINER_MANAGER_H
