//
// Created by Semyon Tikhonenko on 7/3/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "VxApp.h"
#include "VxPitchInputReader.h"

using namespace CppUtils;

VxApp* VxApp::_instance = nullptr;

VxApp *VxApp::instance() {
    return _instance;
}

VxApp::VxApp(VxPitchInputReader* pitchInputReader, MvxPlayer* mvxPlayer) {
    workspaceController = nullptr;
    this->pitchInputReader = pitchInputReader;
    this->mvxPlayer = mvxPlayer;

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

VxApp::~VxApp() {
    delete mvxPlayer;
    delete pitchInputReader;
}

MvxPlayer *VxApp::getMvxPlayer() const {
    return mvxPlayer;
}

void VxApp::initInstance(VxApp *app) {
    assert(!_instance);
    _instance = app;
}

void VxApp::setWorkspaceController(WorkspaceController *workspaceController) {
    this->workspaceController = workspaceController;
    workspaceController->setPitchesCollector(pitchInputReader);
    workspaceController->setIntervalWidth(30);
    workspaceController->setIntervalHeight(15);
    workspaceController->setVxFile(mvxPlayer->getVxFile());
}
