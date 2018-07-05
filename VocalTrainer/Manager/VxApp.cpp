//
// Created by Semyon Tikhonenko on 7/3/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "VxApp.h"
#include "VxPitchInputReader.h"

using namespace CppUtils;

VxApp *VxApp::instance() {
    static VxApp inst;
    return &inst;
}

VxApp::VxApp() {
    workspaceController = nullptr;
    this->pitchInputReader = new VxPitchInputReader();
    this->mvxPlayer = new MvxPlayer();
    this->zoomController = new ZoomController();

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

    zoomController->addZoomChangedListener([this] (float zoom) {
        updateZoom();
        return DONT_DELETE_LISTENER;
    });

    zoomController->addFirstPitchPerfectFrequencyIndexChangedListener([this] (int) {
        updateFirstPerfectFrequencyIndex();
        return DONT_DELETE_LISTENER;
    });
}

void VxApp::updateWorkspaceIsPlayingChanged(bool playing) {
    WorkspaceController* controller = this->workspaceController;
    if (playing) {
        controller->setIntervalsPerSecond(this->mvxPlayer->getBeatsPerMinute() / 60.0);
    } else {
        controller->setIntervalsPerSecond(0);
    }
}

VxPitchInputReader *VxApp::getPitchInputReader() const {
    return pitchInputReader;
}

ZoomController *VxApp::getZoomController() const {
    return zoomController;
}

VxApp::~VxApp() {
    delete mvxPlayer;
    delete pitchInputReader;
}

MvxPlayer *VxApp::getMvxPlayer() const {
    return mvxPlayer;
}

void VxApp::setWorkspaceController(WorkspaceController *workspaceController) {
    this->workspaceController = workspaceController;
    workspaceController->setPitchesCollector(pitchInputReader);
    updateZoom();
    updateFirstPerfectFrequencyIndex();
    workspaceController->setVxFile(mvxPlayer->getVxFile());
}

void VxApp::updateZoom() {
    if (!workspaceController) {
        return;
    }

    WorkspaceController* controller = workspaceController;
    controller->setIntervalWidth(zoomController->getIntervalWidth());
    controller->setIntervalHeight(zoomController->getIntervalHeight());
}

void VxApp::updateFirstPerfectFrequencyIndex() {
    WorkspaceController* controller = workspaceController;
    controller->setFirstPitchPerfectFrequencyIndex(zoomController->getFirstPitchPerfectFrequencyIndex());
}
