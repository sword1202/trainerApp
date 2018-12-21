//
// Created by Semyon Tikhonenko on 7/3/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MainController.h"
#include "AudioInputManager.h"
#include "PlaybackBounds.h"
#include "Executors.h"
#include "WAVFile.h"
#include <iostream>

using namespace CppUtils;
using std::cout;
using std::endl;

static MainController* _instance = nullptr;

MainController *MainController::instance() {
    return _instance;
}

void MainController::initInstance(MainController* inst) {
    assert(!_instance);
    _instance = inst;
}

void MainController::init(AudioInputManager *pitchInputReader, MvxPlayer *mvxPlayer, WorkspaceZoomController *zoomController) {
    this->audioInputManager = pitchInputReader;
    this->mvxPlayer = mvxPlayer;
    this->zoomController = zoomController;

    workspaceController = nullptr;

    mvxPlayer->stopRequestedListeners.addListener([=] {
        onStopPlaybackRequested();
    });

    mvxPlayer->isPlayingChangedListeners.addListener([this] (bool playing) {
        if (playing) {
            this->audioInputManager->startPitchDetection(this->mvxPlayer->getSeek());
        } else {
            this->audioInputManager->stopPitchDetection();
        }

        workspaceController->setRunning(playing);
        updateSeek(this->mvxPlayer->getSeek());
    });

    mvxPlayer->vxFileChangedListeners.addListener([this] (const VxFile* vxFile) {
        if (workspaceController) {
            workspaceController->setVxFile(vxFile);
        }
    });

    mvxPlayer->prepareFinishedListeners.addListener([=] {
        if (workspaceController) {
            workspaceController->setIntervalsPerSecond(this->mvxPlayer->getBeatsPerMinute() / 60.0);
        }
    });

    mvxPlayer->tonalityChangedListeners.addListener([=] {
        if (workspaceController) {
            workspaceController->update();
        }
    });

    mvxPlayer->seekChangedListeners.addListener([=] (double seek) {
        if (!mvxPlayer->isCompleted()) {
            audioInputManager->setPitchesRecorderSeek(seek);
        }
    });

    mvxPlayer->setInstrumentalVolume(1.0);
    mvxPlayer->setPianoVolume(0.5);
    pitchInputReader->setOutputVolume(0.0);

    pitchInputReader->getPitchDetectedListeners().addListener([=] (const Pitch& pitch, double) {
        workspaceController->setDetectedPitch(pitch);
    });
}

void MainController::onStopPlaybackRequested() {
    workspaceController->setRunning(false);
    updateSeek(mvxPlayer->getSeek());
}

AudioInputManager *MainController::getAudioInputManager() const {
    return audioInputManager;
}

WorkspaceZoomController *MainController::getZoomController() const {
    return zoomController;
}

MainController::~MainController() {
    delete mvxPlayer;
    delete audioInputManager;
    if (playbackBoundsSelectionController) {
        delete playbackBoundsSelectionController;
    }
}

MvxPlayer *MainController::getPlayer() const {
    return mvxPlayer;
}

void MainController::setWorkspaceController(WorkspaceController *workspaceController) {
    assert(!this->workspaceController);
    this->workspaceController = workspaceController;
    workspaceController->setPitchesRecorder(audioInputManager->getPitchesRecorder());
    workspaceController->setVxFile(mvxPlayer->getVxFile());
    workspaceController->setIntervalsPerSecond(mvxPlayer->getBeatsPerMinute() / 60.0);
    workspaceController->setPitchSequence(mvxPlayer);

    updateZoom();
    updateWorkspaceFirstPitch();

    Executors::ExecuteOnMainThread([this] {

        this->workspaceController->setSummarizedGridHeight(zoomController->getSummarizedWorkspaceGridHeight());
        this->workspaceController->setVerticalScrollPosition(zoomController->getVerticalScrollPosition());

        mvxPlayer->seekChangedFromUserListeners.addListener([=] (double seek) {
            updateSeek(seek);
            if (!mvxPlayer->isCompleted()) {
                audioInputManager->setAudioRecorderSeek(seek);
            }
        });

        mvxPlayer->boundsChangedListeners.addListener([=] (const PlaybackBounds& bounds) {
            this->workspaceController->setPlaybackBounds(bounds);
            this->workspaceController->update();
        });
        this->workspaceController->setPlaybackBounds(mvxPlayer->getBounds());

        zoomController->summarizedWorkspaceGridHeightChangedListeners.addListener([=] {
            this->workspaceController->setSummarizedGridHeight(zoomController->getSummarizedWorkspaceGridHeight());
        });

        zoomController->firstPitchChangedListeners.addListener([this](const Pitch&) {
            updateWorkspaceFirstPitch();
        });

        zoomController->verticalScrollPositionChangedListeners.addListener([=] (float value) {
            this->workspaceController->setVerticalScrollPosition(value);
            this->workspaceController->update();
        });

        zoomController->zoomChangedListeners.addListener([this] (float zoom) {
            updateZoom();
        });

        playbackBoundsSelectionController = new PlaybackBoundsSelectionController(this->workspaceController, mvxPlayer);
    });
}

void MainController::updateSeek(double seek) {
    double intervalsPerSecond = workspaceController->getIntervalsPerSecond();
    float intervalWidth = zoomController->getIntervalWidth();
    double horizontalOffset = intervalsPerSecond * seek * intervalWidth;
    workspaceController->setHorizontalOffset((float)horizontalOffset);
    workspaceController->update();
}

void MainController::updateZoom() {
    workspaceController->setIntervalWidth(zoomController->getIntervalWidth());
    workspaceController->setIntervalHeight(zoomController->getIntervalHeight());
    updateSeek(mvxPlayer->getSeek());
}

void MainController::updateWorkspaceFirstPitch() {
    workspaceController->setFirstVisiblePitch(zoomController->getFirstPitch());
}

PlaybackBoundsSelectionController *MainController::getPlaybackBoundsSelectionController() const {
    assert(playbackBoundsSelectionController != nullptr);
    return playbackBoundsSelectionController;
}

void MainController::generateRecording(MvxFile *out) {
    out->setArtistNameUtf8(mvxPlayer->getArtistNameUtf8());
    out->setSongTitleUtf8(mvxPlayer->getSongTitleUtf8());

    // save recorded .wav audio data
    std::string recording = audioInputManager->getRecordedDataInWavFormat();
    out->setRecordingData(recording);

    // save instrumental
    out->setInstrumental(mvxPlayer->getInstrumental());
    out->setBeatsPerMinute(mvxPlayer->getBeatsPerMinute());

    // save recorded pitches, to display a graph in the future
    AudioInputPitchesRecorder *pitchesRecorder = audioInputManager->getPitchesRecorder();
    out->setRecordedPitchesTimes(pitchesRecorder->getTimesInAscAddedTimeOrder());
    out->setRecordedPitchesFrequencies(pitchesRecorder->getFrequenciesInAscAddedTimeOrder());
}

void MainController::saveRecordingIntoFile(const char *filePath) {
    MvxFile mvxFile;
    generateRecording(&mvxFile);
    mvxFile.writeToFile(filePath);
}
