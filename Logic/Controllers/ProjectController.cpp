//
// Created by Semyon Tykhonenko on 2019-07-12.
//

#include "ProjectController.h"
#include "ApplicationModel.h"

ProjectController::ProjectController(ProjectControllerDelegate* delegate) : delegate(delegate) {
    auto* model = ApplicationModel::instance();
    player = model->getPlayer();
    audioInputManager = model->getAudioInputManager();

    player->stopRequestedListeners.addListener([=] {
        onStopPlaybackRequested();
    });

    player->isPlayingChangedListeners.addListener([this] (bool playing) {
        if (!this->player->isRecording()) {
            if (playing) {
                this->audioInputManager->startPitchDetection(this->player->getSeek());
            } else {
                this->audioInputManager->stopPitchDetection();
            }
        }

        if (workspaceController) {
            workspaceController->setRunning(playing);
        }
        updateSeek(this->player->getSeek());
    });

    player->vocalPartChangedListeners.addListener([this] (const VocalPart* vocalPart) {
        if (workspaceController) {
            workspaceController->setVocalPart(vocalPart);
        }
    });

    player->tonalityChangedListeners.addListener([=] {
        if (workspaceController) {
            workspaceController->update();
        }
    });

    player->seekChangedListeners.addListener([=] (double seek) {
        if (!player->isCompleted()) {
            audioInputManager->setPitchesRecorderSeek(seek);
        }
    });

    player->setInstrumentalVolume(1.0);
    player->setVocalPartPianoVolume(0.5);
    audioInputManager->setOutputVolume(0.0);

    audioInputManager->getPitchDetectedListeners().addListener([=] (const Pitch& pitch, double) {
        workspaceController->setDetectedPitch(pitch);
    });
}

void ProjectController::onStopPlaybackRequested() {
    if (workspaceController) {
        workspaceController->setRunning(false);
    }
    updateSeek(player->getSeek());
}

void ProjectController::updateSeek(double seek) {
    if (workspaceController) {
        workspaceController->updateSeek(float(seek));
        workspaceController->update();
    }
    delegate->updateSeek(seek);
}

void ProjectController::play() {
    player->play();
}

void ProjectController::stop() {
    player->pause();
}

std::string ProjectController::getArtistNameUtf8() {
    return player->getArtistNameUtf8();
}

std::string ProjectController::getSongTitleUtf8() {
    return player->getSongTitleUtf8();
}

void ProjectController::setBoundsSelectionEnabled(bool boundsSelectionEnabled) {
    this->boundsSelectionEnabled = boundsSelectionEnabled;

    if (!boundsSelectionEnabled) {
        player->setBounds(PlaybackBounds());
    } else {
        selectedBounds.setStartSeek(workspaceController->getWorkspaceSeek());
    }
}

void ProjectController::onWorkspaceMouseMove(float x) {
    if (!boundsSelectionEnabled) {
        return;
    }

    float seek = workspaceController->getSeekFromXPositionOnWorkspace(x);
    float minimumSeek = player->getTactDuration();
    seek = std::max(seek, minimumSeek);
    selectedBounds.setEndSeek(seek);
    workspaceController->setPlaybackBounds(selectedBounds);
    workspaceController->update();
}

void ProjectController::onWorkspaceMouseClick(float x) {
    if (!boundsSelectionEnabled) {
        return;
    }

    boundsSelectionEnabled = false;
    player->setBounds(selectedBounds);
}

void ProjectController::setVocalVolume(float value) {
    audioInputManager->setOutputVolume(value);
    delegate->updateVocalVolume(audioInputManager->getOutputVolume());
}

void ProjectController::setInputSensitivity(float value) {
    audioInputManager->setInputSensitivity(value);
    delegate->updateInputSensitivity(audioInputManager->getInputSensitivity());
}

void ProjectController::setVocalPianoVolume(float value) {
    player->setVocalPartPianoVolume(value);
    delegate->updateVocalPianoVolume(player->getVocalPianoVolume());
}

void ProjectController::setInstrumentalVolume(float value) {
    player->setInstrumentalVolume(value);
    delegate->updateInstrumentalVolume(player->getInstrumentalVolume());
}

void ProjectController::setWorkspaceController(WorkspaceController *workspaceController) {
    assert(!this->workspaceController && workspaceController);
    this->workspaceController = workspaceController;

    workspaceController->setVocalPart(player->getVocalPart());
    workspaceController->setIntervalsPerSecond(player->getBeatsPerMinute() / 60.0);
    workspaceController->setTotalDurationInSeconds(player->getDuration());
    workspaceController->setInstrumentalTrackSamples(
            this->player->getMvxFile().getInstrumentalPreviewSamples());
    workspaceController->setPitchSequence(player);
    bool isRecording = player->isRecording();
    workspaceController->setRecording(isRecording);
    if (isRecording) {
        workspaceController->setPitchesCollection(player->getPitchesCollection());
    } else {
        workspaceController->setPitchesCollection(audioInputManager->getRecordedPitches());
    }

    player->seekChangedFromUserListeners.addListener([=] (double seek) {
        if (!player->isCompleted()) {
            audioInputManager->setAudioRecorderSeek(seek);
            updateSeek(seek);
        }
    });

    player->boundsChangedListeners.addListener([=] (const PlaybackBounds& bounds) {
        this->workspaceController->setPlaybackBounds(bounds);
        this->workspaceController->update();
    });
    this->workspaceController->setPlaybackBounds(player->getBounds());


    workspaceController->setSeekUpdatedInsideListener([=] (float seek) {
        player->setSeek(seek);
    });
}
