//
// Created by Semyon Tikhonenko on 7/3/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MainController.h"
#include "PitchInputReaderAndPlayer.h"
#include "PlaybackBounds.h"
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

void MainController::init(PitchInputReaderAndPlayer *pitchInputReader, MvxPlayer *mvxPlayer, ZoomController *zoomController) {
    this->pitchInputReader = pitchInputReader;
    this->mvxPlayer = mvxPlayer;
    this->zoomController = zoomController;

    workspaceController = nullptr;

    mvxPlayer->stopRequestedListeners.addListener([=] {
        onStopPlaybackRequested();
        return DONT_DELETE_LISTENER;
    });

    mvxPlayer->isPlayingChangedListeners.addListener([this] (bool playing) {
        if (playing) {
            this->pitchInputReader->start();
        } else {
            this->pitchInputReader->stop();
        }

        workspaceController->setRunning(playing);
        updateSeek(this->mvxPlayer->getSeek());

        return DONT_DELETE_LISTENER;
    });

    mvxPlayer->vxFileChangedListeners.addListener([this] (const VxFile* vxFile) {
        workspaceController->setVxFile(vxFile);
        return DONT_DELETE_LISTENER;
    });

    mvxPlayer->prepareFinishedListeners.addListener([=] {
        workspaceController->setIntervalsPerSecond(this->mvxPlayer->getBeatsPerMinute() / 60.0);
        return DONT_DELETE_LISTENER;
    });

    mvxPlayer->tonalityChangedListeners.addListener([=] {
        workspaceController->update();
        return DONT_DELETE_LISTENER;
    });

    mvxPlayer->setInstrumentalVolume(1.0);
    mvxPlayer->setPianoVolume(0.5);

    zoomController->zoomChangedListeners.addListener([this] (float zoom) {
        updateZoom();
        return DONT_DELETE_LISTENER;
    });

    pitchInputReader->addPitchDetectedListener([=] (const Pitch& pitch, double) {
        workspaceController->setDetectedPitch(pitch);
        return DONT_DELETE_LISTENER;
    });
}

void MainController::onStopPlaybackRequested() {
    workspaceController->setRunning(false);
    updateSeek(mvxPlayer->getSeek());
}

PitchInputReaderAndPlayer *MainController::getPitchInputReader() const {
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
    assert(!this->workspaceController);
    this->workspaceController = workspaceController;
    workspaceController->setPitchesCollector(pitchInputReader);
    updateZoom();
    updateWorkspaceFirstPitch();
    workspaceController->setVxFile(mvxPlayer->getVxFile());
    workspaceController->setPitchSequence(mvxPlayer);
    workspaceController->setSummarizedGridHeight(zoomController->getSummarizedWorkspaceGridHeight());
    workspaceController->setVerticalScrollPosition(zoomController->getVerticalScrollPosition());
    
    mvxPlayer->seekChangedFromUserListeners.addListener([=] (double seek) {
        updateSeek(seek);
        return DONT_DELETE_LISTENER;
    });

    mvxPlayer->boundsChangedListeners.addListener([=] (const PlaybackBounds& bounds) {
        workspaceController->setPlaybackBounds(bounds);
        workspaceController->update();
        return DONT_DELETE_LISTENER;
    });
    workspaceController->setPlaybackBounds(mvxPlayer->getBounds());

    zoomController->summarizedWorkspaceGridHeightChangedListeners.addListener([=] {
        workspaceController->setSummarizedGridHeight(zoomController->getSummarizedWorkspaceGridHeight());
        return DONT_DELETE_LISTENER;
    });

    zoomController->firstPitchChangedListeners.addListener([this](const Pitch&) {
        updateWorkspaceFirstPitch();
        return DONT_DELETE_LISTENER;
    });

    zoomController->verticalScrollPositionChangedListeners.addListener([=] (float value) {
        workspaceController->setVerticalScrollPosition(value);
        workspaceController->update();
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
        workspaceController->setIntervalWidth(zoomController->getIntervalWidth());
        workspaceController->setIntervalHeight(zoomController->getIntervalHeight());
        updateSeek(mvxPlayer->getSeek());
    }
}

void MainController::updateWorkspaceFirstPitch() {
    workspaceController->setFirstVisiblePitch(zoomController->getFirstPitch());

}
