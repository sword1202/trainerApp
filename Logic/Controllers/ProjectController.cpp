//
// Created by Semyon Tykhonenko on 2019-07-12.
//

#include "ProjectController.h"
#include "ApplicationModel.h"
#include "Primitives.h"

using namespace CppUtils;

ProjectController::ProjectController(ProjectControllerDelegate* delegate) : delegate(delegate) {
    auto* model = ApplicationModel::instance();
    player = model->getPlayer();
    audioInputManager = model->getAudioInputManager();

    player->stopRequestedListeners.addListener([=] {
        onStopPlaybackRequested();
    }, this);

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
        if (playing) {
            this->delegate->onPlaybackStarted();
        } else {
            this->delegate->onPlaybackStopped();
        }
        updateSeek(this->player->getSeek());
    }, this);

    player->vocalPartChangedListeners.addListener([this] (const VocalPart* vocalPart) {
        if (workspaceController) {
            workspaceController->setVocalPart(vocalPart);
        }
    }, this);

    player->tonalityChangedListeners.addListener([=] {
        if (workspaceController) {
            workspaceController->update();
        }
    }, this);

    player->seekChangedListeners.addListener([=] (double seek) {
        if (!player->isCompleted()) {
            audioInputManager->setPitchesRecorderSeek(seek);
        }
    }, this);

    player->setInstrumentalVolume(1.0);
    player->setVocalPartPianoVolume(2.0);
    audioInputManager->setOutputVolume(0.0);

    audioInputManager->getPitchDetectedListeners().addListener([=] (const Pitch& pitch, double) {
        workspaceController->setDetectedPitch(pitch);
    }, this);

    audioInputManager->addAudioInputLevelMonitor([=] (double level) {
        delegate->updateAudioLevel(level);
    }, this);

    player->lyricsChangedListeners.addListener([=] {
        delegate->onHasLyricsChanged(player->hasLyrics());
    }, this);

    player->currentLyricsLinesChangedListeners.addListener([=] {
        delegate->updateLyricsText(player->getLyricsTextForPart(0));
    }, this);

    rewinder = new Rewinder(player);
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

const std::string & ProjectController::getArtistNameUtf8() {
    return player->getArtistNameUtf8();
}

const std::string & ProjectController::getSongTitleUtf8() {
    return player->getSongTitleUtf8();
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

    workspaceController->setDelegate(this);
    workspaceController->setVocalPart(player->getVocalPart());
    workspaceController->setBeatsPerSecond(player->getBeatsPerMinute() / 60.0);
    workspaceController->setTotalDurationInSeconds(player->getDuration());
    workspaceController->setInstrumentalTrackSamples(
            this->player->getFile().getInstrumentalPreviewSamples());
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

    delegate->onTracksVisibilityChanged(isTracksVisible());
}

void ProjectController::setLyricsVisible(bool value) {
    lyricsVisible = value;
    delegate->onLyricsVisibilityChanged(value);
}

void ProjectController::setMetronomeEnabled(bool value) {
    if (value == isMetronomeEnabled()) {
        return;
    }

    player->setMetronomeEnabled(value);
    delegate->onMetronomeEnabledChanged(value);
}

void ProjectController::setTracksVisible(bool value) {
    if (value == isTracksVisible()) {
        return;
    }

    workspaceController->setDrawTracks(value);
    delegate->onTracksVisibilityChanged(value);
}

bool ProjectController::isLyricsVisible() const {
    return lyricsVisible;
}

bool ProjectController::isTracksVisible() const {
    if (!workspaceController) {
        return false;
    }

    return workspaceController->shouldDrawTracks();
}

bool ProjectController::isMetronomeEnabled() const {
    return player->isMetronomeEnabled();
}

void ProjectController::setZoom(float zoom) {
    assert(workspaceController && "setZoom has been called before WorkspaceController initialized");
    if (Primitives::CompareFloats(workspaceController->getZoom(), zoom)) {
        return;
    }

    workspaceController->setZoom(zoom);
    delegate->onZoomChanged(zoom);
}

float ProjectController::getZoom() const {
    return workspaceController == nullptr ? getMinZoom() : workspaceController->getZoom();
}

void ProjectController::onPlaybackBoundsChangedByUserEvent(const PlaybackBounds &newBounds) {
    player->setBounds(newBounds);
}

void ProjectController::onSeekChangedByUserEvent(float newSeek) {
    player->setSeek(newSeek);
}

float ProjectController::getMinZoom() const {
    return workspaceController->getMinZoom();
}

float ProjectController::getMaxZoom() const {
    return workspaceController->getMaxZoom();
}

void ProjectController::setBoundsSelectionEnabled(bool boundsSelectionEnabled) {
    assert(workspaceController && "call setWorkspaceController before setBoundsSelectionEnabled");
    workspaceController->setBoundsSelectionEnabled(boundsSelectionEnabled);
}

bool ProjectController::isBoundsSelectionEnabled() const {
    return workspaceController == nullptr ? false : workspaceController->isBoundsSelectionEnabled();
}

void ProjectController::togglePlay() {
    if (isPlaying()) {
        stop();
    } else {
        play();
    }
}

bool ProjectController::isPlaying() const {
    return player->isPlaying();
}

void ProjectController::toggleRewind(bool backward) {
    bool isCurrentRewindBackward;
    if (rewinder->isRewindRunning(&isCurrentRewindBackward)) {
        rewinder->stopRewind();
        delegate->onRewindStatusChanged(false, isCurrentRewindBackward);
        if (isCurrentRewindBackward != backward) {
            rewinder->startRewind(backward);
            delegate->onRewindStatusChanged(true, backward);
        }
    } else {
        rewinder->startRewind(backward);
        delegate->onRewindStatusChanged(true, backward);
    }
}

void ProjectController::goToBeginning() {
    player->stopAndMoveSeekToBeginning();
}
