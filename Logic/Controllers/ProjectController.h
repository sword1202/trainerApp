//
// Created by Semyon Tykhonenko on 2019-07-12.
//

#ifndef TEXTIMAGESGENERATOR_PROJECTCONTROLLER_H
#define TEXTIMAGESGENERATOR_PROJECTCONTROLLER_H

#include <string>
#include "WorkspaceController.h"
#include "MvxPlayer.h"
#include "AudioInputManager.h"

class ProjectControllerDelegate {
public:
    virtual inline void updateAudioLevel(double level) {}
    virtual inline void updateSeek(double seek) {}
    virtual inline void onPlaybackStarted() {}
    virtual inline void onPlaybackStopped() {}
    virtual inline void onHasLyricsChanged(bool hasLyrics) {}
    virtual inline void updateLyricsLine(const std::string& lyricsLineUtf8) {}
    virtual inline void onShowLyricsChanged(bool showLyrics) {}
    virtual inline void onMetronomeEnabledChanged(bool enabled) {}
    virtual inline void updateVocalPianoVolume(float volume) {}
    virtual inline void updateInstrumentalVolume(float volume) {}
    virtual inline void updateVocalVolume(float volume) {}
    virtual inline void updateInputSensitivity(float value) {}
    virtual inline void updateSaveIndicator(bool hasSaveIndicator) {}
};

class ProjectController {
    WorkspaceController* workspaceController = nullptr;
    ProjectControllerDelegate* delegate = nullptr;
    MvxPlayer* player;
    AudioInputManager* audioInputManager;
    bool boundsSelectionEnabled = false;
    PlaybackBounds selectedBounds;

    void onStopPlaybackRequested();
    void updateSeek(double seek);
public:
    explicit ProjectController(ProjectControllerDelegate* delegate);

    void setWorkspaceController(WorkspaceController *workspaceController);

    std::string getArtistNameUtf8();
    std::string getSongTitleUtf8();

    void play();
    void stop();

    // Bounds
    void setBoundsSelectionEnabled(bool boundsSelectionEnabled);
    void onWorkspaceMouseMove(float x);
    void onWorkspaceMouseClick(float x);

    void setVocalVolume(float value);
    void setInputSensitivity(float value);
    void setVocalPianoVolume(float value);
    void setInstrumentalVolume(float value);
};


#endif //TEXTIMAGESGENERATOR_PROJECTCONTROLLER_H
