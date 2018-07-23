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

    mvxPlayer->addIsPlayingChangedListener([this] (bool playing) {
        if (playing) {
            this->pitchInputReader->start();
        } else {
            this->pitchInputReader->stop();
        }

        updateWorkspaceIsPlayingChanged(playing);

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

    mvxPlayer->setInstrumentalVolume(1.0);
    mvxPlayer->setPianoVolume(0.5);

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

void MainController::updateWorkspaceIsPlayingChanged(bool playing) {
    WorkspaceController* controller = this->workspaceController;
    if (playing) {
        updateSeek(mvxPlayer->getSeek());
    }
    controller->setRunning(playing);
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
    
    mvxPlayer->addSeekChangedListener([=] (double seek) {
        //updateSeek(seek);
        return DONT_DELETE_LISTENER;
    });
}

void MainController::updateSeek(double seek) {
    WorkspaceController* workspaceController = this->workspaceController;
    double intervalsPerSecond = workspaceController->getIntervalsPerSecond();
    float intervalWidth = zoomController->getIntervalWidth();
    double horizontalOffset = intervalsPerSecond * seek * intervalWidth;
    workspaceController->setHorizontalOffset((float)horizontalOffset);
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
