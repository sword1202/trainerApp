//
// Created by Semyon Tikhonenko on 7/3/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MainController.h"
#include "VxPitchInputReader.h"
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

MainController::MainController(VxPitchInputReader *pitchInputReader, MvxPlayer *mvxPlayer, ZoomController *zoomController)
        : pitchInputReader(pitchInputReader), mvxPlayer(mvxPlayer), zoomController(zoomController) {
    workspaceController = nullptr;

    mvxPlayer->addStopRequestedListener([=] {
        onStopPlaybackRequested();
        return DONT_DELETE_LISTENER;
    });

    mvxPlayer->addIsPlayingChangedListener([this] (bool playing) {
        if (playing) {
            this->pitchInputReader->start();
        } else {
            this->pitchInputReader->stop();
        }

        WorkspaceController* controller = workspaceController;
        controller->setRunning(playing);
        updateSeek(this->mvxPlayer->getSeek());

        return DONT_DELETE_LISTENER;
    });

    mvxPlayer->addVxFileChangedListener([this] (const VxFile* vxFile) {
        WorkspaceController* controller = workspaceController;
        controller->setVxFile(vxFile);
        return DONT_DELETE_LISTENER;
    });

    mvxPlayer->addPrepareFinishedListener([=] {
        WorkspaceController* controller = workspaceController;
        controller->setIntervalsPerSecond(this->mvxPlayer->getBeatsPerMinute() / 60.0);
        return DONT_DELETE_LISTENER;
    });

    mvxPlayer->addTonalityChangedListener([=] {
        WorkspaceController* controller = workspaceController;
        controller->update();
        return DONT_DELETE_LISTENER;
    });

    mvxPlayer->setInstrumentalVolume(0.0);
    mvxPlayer->setPianoVolume(0.0);

    zoomController->addZoomChangedListener([this] (float zoom) {
        updateZoom();
        return DONT_DELETE_LISTENER;
    });

    zoomController->addFirstPitchChangedListener([this](Pitch) {
        updateWorkspaceFirstPitch();
        pianoController->setFirstPitch(this->zoomController->getFirstPitch());
        onPianoUpdateRequested();
        return DONT_DELETE_LISTENER;
    });

    pitchInputReader->addPitchDetectedListener([=] (const Pitch& pitch, double) {
        if (pianoController) {
            pianoController->setDetectedPitch(pitch);
            onPianoUpdateRequested();
        }
        return DONT_DELETE_LISTENER;
    });
}

void MainController::onStopPlaybackRequested() {
    WorkspaceController* controller = this->workspaceController;
    controller->setRunning(false);
    updateSeek(mvxPlayer->getSeek());
}

VxPitchInputReader *MainController::getPitchInputReader() const {
    return pitchInputReader;
}

ZoomController *MainController::getZoomController() const {
    return zoomController;
}

MainController::~MainController() {
    delete mvxPlayer;
    delete pitchInputReader;
}

MvxPlayer *MainController::getPlayer() const {
    return mvxPlayer;
}

void MainController::setWorkspaceController(WorkspaceController *workspaceController) {
    this->workspaceController = workspaceController;
    workspaceController->setPitchesCollector(pitchInputReader);
    updateZoom();
    updateWorkspaceFirstPitch();
    workspaceController->setVxFile(mvxPlayer->getVxFile());
    
    mvxPlayer->addSeekChangedFromUserListener([=] (double seek) {
        updateSeek(seek);
        return DONT_DELETE_LISTENER;
    });
}

void MainController::updateSeek(double seek) {
    WorkspaceController* workspaceController = this->workspaceController;
    double intervalsPerSecond = workspaceController->getIntervalsPerSecond();
    float intervalWidth = zoomController->getIntervalWidth();
    double horizontalOffset = intervalsPerSecond * seek * intervalWidth;
    workspaceController->setHorizontalOffset((float)horizontalOffset);
    workspaceController->update();
}

void MainController::updateZoom() {
    if (workspaceController) {
        WorkspaceController* controller = workspaceController;
        controller->setIntervalWidth(zoomController->getIntervalWidth());
        controller->setIntervalHeight(zoomController->getIntervalHeight());
        updateSeek(mvxPlayer->getSeek());
    }

    if (pianoController) {
        pianoController->setIntervalHeight(zoomController->getIntervalHeight());
        onPianoUpdateRequested();
    }
}

void MainController::updateWorkspaceFirstPitch() {
    WorkspaceController* controller = workspaceController;
    controller->setFirstVisiblePitch(zoomController->getFirstPitch());
}

void MainController::setPianoController(PianoController *pianoController,
        const std::function<void()>& onUpdateRequested) {
    assert(pianoController);
    onPianoUpdateRequested = onUpdateRequested;
    this->pianoController = pianoController;
    pianoController->setDetectedPitch(pitchInputReader->getLastDetectedPitch());
    pianoController->setFirstPitch(zoomController->getFirstPitch());
    pianoController->setIntervalHeight(zoomController->getIntervalHeight());
}
