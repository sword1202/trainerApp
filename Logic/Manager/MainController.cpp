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

void MainController::init(AudioInputManager *pitchInputReader, MvxPlayer *mvxPlayer) {
    this->audioInputManager = pitchInputReader;
    this->mvxPlayer = mvxPlayer;

    workspaceController = nullptr;

    mvxPlayer->stopRequestedListeners.addListener([=] {
        onStopPlaybackRequested();
    });

    mvxPlayer->isPlayingChangedListeners.addListener([this] (bool playing) {
        if (!this->mvxPlayer->isRecording()) {
            if (playing) {
                this->audioInputManager->startPitchDetection(this->mvxPlayer->getSeek());
            } else {
                this->audioInputManager->stopPitchDetection();
            }
        }

        workspaceController->setRunning(playing);
        updateSeek(this->mvxPlayer->getSeek());
    });

    mvxPlayer->vxFileChangedListeners.addListener([this] (const VxFile* vxFile) {
        if (workspaceController) {
            workspaceController->setVxFile(vxFile);
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

MainController::~MainController() {
    delete mvxPlayer;
    delete audioInputManager;
    delete playbackBoundsSelectionController;
}

MvxPlayer *MainController::getPlayer() const {
    return mvxPlayer;
}

void MainController::setWorkspaceController(WorkspaceController *workspaceController) {
    assert(!this->workspaceController);
    workspaceController->setVxFile(mvxPlayer->getVxFile());
    workspaceController->setIntervalsPerSecond(mvxPlayer->getBeatsPerMinute() / 60.0);
    workspaceController->setTotalDurationInSeconds(mvxPlayer->getDuration());
    workspaceController->setInstrumentalTrackSamples(
            this->mvxPlayer->getMvxFile().getInstrumentalPreviewSamples());
    workspaceController->setPitchSequence(mvxPlayer);
    bool isRecording = mvxPlayer->isRecording();
    workspaceController->setRecording(isRecording);
    if (isRecording) {
        workspaceController->setPitchesCollection(mvxPlayer->getPitchesCollection());
    } else {
        workspaceController->setPitchesCollection(audioInputManager->getRecordedPitches());
    }

    Executors::ExecuteOnMainThread([this, workspaceController] {
        this->workspaceController = workspaceController;

        mvxPlayer->seekChangedFromUserListeners.addListener([=] (double seek) {
            if (!mvxPlayer->isCompleted()) {
                audioInputManager->setAudioRecorderSeek(seek);
            }
        });

        mvxPlayer->boundsChangedListeners.addListener([=] (const PlaybackBounds& bounds) {
            this->workspaceController->setPlaybackBounds(bounds);
            this->workspaceController->update();
        });
        this->workspaceController->setPlaybackBounds(mvxPlayer->getBounds());

        playbackBoundsSelectionController = new PlaybackBoundsSelectionController(this->workspaceController, mvxPlayer);

        workspaceControllerReadyCallbacksQueue.process();
        workspaceController->setSeekUpdatedInsideListener([=] (float seek) {
            mvxPlayer->setSeek(seek);
        });
    });
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
    out->setVxFile(*mvxPlayer->getVxFile());
    out->setBeatsPerMinute(mvxPlayer->getBeatsPerMinute());

    // save recorded pitches, to display a graph in the future
    const PitchesCollection *pitches = audioInputManager->getRecordedPitches();
    out->setRecordedPitchesTimes(pitches->getTimes());
    out->setRecordedPitchesFrequencies(pitches->getFrequencies());
}

void MainController::saveRecordingIntoFile(const char *filePath) {
    MvxFile mvxFile;
    generateRecording(&mvxFile);
    mvxFile.writeToFile(filePath);
}

void MainController::getWorkspaceController(const std::function<void(WorkspaceController *)> &callback) {
    if (workspaceController) {
        callback(workspaceController);
    } else {
        workspaceControllerReadyCallbacksQueue.post([=] {
            callback(workspaceController);
        });
    }
}

void MainController::updateSeek(double seek) {
    workspaceController->updateSeek(float(seek));
    workspaceController->update();
}
