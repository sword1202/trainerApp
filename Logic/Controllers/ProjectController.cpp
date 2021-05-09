//
// Created by Semyon Tykhonenko on 2019-07-12.
//

#include "ProjectController.h"
#include "ApplicationModel.h"
#include "Primitives.h"
#include "Executors.h"
#include <iostream>

using namespace CppUtils;
using std::cout;
using std::endl;

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
        updateWorkspaceSeek(this->player->getSeek());
    }, this);

    player->vocalPartChangedListeners.addListener([this] (const VocalPart* vocalPart) {
        if (workspaceController) {
            workspaceController->setVocalPart(vocalPart, player->getBeatsPerSecond());
            updateWorkspaceSeek(player->getSeek());
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
        Executors::ExecuteOnMainThread([=] {
            this->delegate->updateSeek(seek);
        });
    }, this);

    player->setInstrumentalVolume(1.0);
    player->setVocalPartPianoVolume(1.0);
    audioInputManager->setOutputVolume(0.0);

    audioInputManager->getPitchDetectedListeners().addListener([=] (const Pitch& pitch, double) {
        if (workspaceController) {
            workspaceController->setDetectedPitch(pitch);
        }
    }, this);

    audioInputManager->addAudioInputLevelMonitor([=] (double level) {
        delegate->updateAudioLevel(level);
    }, this);

    player->lyricsSelectionChangedListeners.addListener([=] (const LyricsPlayer::Selection& selection) {
        delegate->updateLyricsSelection(selection);
    }, this);

    player->currentLyricsLinesChangedListeners.addListener([=] (const LyricsDisplayedLinesProvider* provider) {
        delegate->updateLyricsLines(provider);
    }, this);

    player->boundsChangedListeners.addListener([=] (const PlaybackBounds& bounds) {
        delegate->updateEndSeek(getEndSeek());
        delegate->updateSeek(player->getSeek());
    }, this);

    player->onCompleteListeners.addListener([=] {
        delegate->onPlaybackCompleted();
    }, this);

    rewinder = new Rewinder(player);
}

void ProjectController::onStopPlaybackRequested() {
    if (workspaceController) {
        workspaceController->setRunning(false);
    }
    updateWorkspaceSeek(player->getSeek());
}

void ProjectController::updateWorkspaceSeek(double seek) {
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
    if (value == player->getVocalPianoVolume()) {
        return;
    }

    player->setVocalPartPianoVolume(value);
    delegate->updateVocalPianoVolume(player->getVocalPianoVolume());
}

void ProjectController::setInstrumentalVolume(float value) {
    if (value == player->getInstrumentalVolume()) {
        return;
    }

    player->setInstrumentalVolume(value);
    delegate->updateInstrumentalVolume(player->getInstrumentalVolume());
}

float ProjectController::getVocalVolume() const {
    return audioInputManager->getOutputVolume();
}

float ProjectController::getInputSensitivity() const {
    return audioInputManager->getInputSensitivity();
}

float ProjectController::getVocalPianoVolume() const {
    return player->getVocalPianoVolume();
}

float ProjectController::getInstrumentalVolume() const {
    return player->getInstrumentalVolume();
}

void ProjectController::setWorkspaceController(WorkspaceController *workspaceController) {
    assert(!this->workspaceController && workspaceController);
    this->workspaceController = workspaceController;

    workspaceController->setDelegate(this);
    workspaceController->setVocalPart(player->getVocalPart(), player->getBeatsPerSecond());
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
            updateWorkspaceSeek(seek);
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
    if (value == lyricsVisible) {
        return;
    }

    lyricsVisible = value;
    delegate->updateLyricsVisibilityChanged(value);
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
    delegate->updateZoom(zoom);
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

void ProjectController::setZoom(float zoom, const PointF& intoPoint) {
    workspaceController->setZoom(zoom, intoPoint);
}

float ProjectController::getVerticalScrollPosition() const {
    return workspaceController->getVerticalScrollPosition();
}

void ProjectController::setVerticalScrollPosition(float position) {
    workspaceController->setVerticalScrollPosition(position);
}

void ProjectController::updateDelegate() {
    if (workspaceController) {
        delegate->updateZoom(workspaceController->getZoom());
    }
    const LyricsDisplayedLinesProvider *lyricsLines = player->getDisplayedLyricsLines();
    if (lyricsLines) {
        delegate->updateLyricsLines(lyricsLines);
    }
}

void ProjectController::setPlaybackProgress(double value) {
    assert(value >= 0 && value <= 1);
    const PlaybackBounds& bounds = player->getBounds();
    double seek;
    if (bounds) {
        seek = bounds.getDuration() * value + bounds.getStartSeek();
    } else {
        seek = player->getDuration() * value;
    }
    player->setSeek(seek);
}

double ProjectController::convertSeekToPlaybackProgress(double seek) const {
    assert(seek >= 0);
    const PlaybackBounds& bounds = player->getBounds();
    double duration = bounds ? bounds.getDuration() : player->getDuration();
    seek = bounds ? seek - bounds.getStartSeek() : seek;
    if (seek < 0) {
        seek = 0;
    }

    double result = seek / duration;
    if (result > 1.0) {
        return 1.0;
    } else {
        return result;
    }
}

const std::vector<Lyrics::Section> & ProjectController::getLyricsSections() const {
    const PlaybackBounds &bounds = player->getBounds();
    if (bounds) {
        auto range = player->getFile().getLyrics().getSectionsInTimeRange(bounds.getStartSeek(), bounds.getEndSeek());
        sections.assign(range.first, range.second);
    } else {
        sections.assign(player->getLyricsSections().begin(), player->getLyricsSections().end());
    }

    return sections;
}

const Tonality &ProjectController::getOriginalTonality() const {
    return player->getFile().getOriginalTonality();
}

void ProjectController::setPitchShift(int value) {
    if (value == player->getPitchShiftInSemiTones()) {
        return;
    }

    player->setPitchShiftInSemiTones(value);
    delegate->updateTonality(value);
}

double ProjectController::getBeatsPerMinute() const {
    return player->getBeatsPerMinute();
}

double ProjectController::getOriginalBeatsPerMinute() const {
    return player->getOriginalBeatsPerMinute();
}

void ProjectController::setTempoFactor(double value) {
    assert(!player->isRecording() && "tempoFactor change is not allowed while recording playback");
    if (Primitives::CompareFloatsUsingEpsilon(value, player->getTempoFactor(), 0.000001)) {
        return;
    }

    audioInputManager->clearRecordedData();
    player->setTempoFactor(value);
    delegate->updateTempoFactor(value);
}

double ProjectController::getEndSeek() const {
    const auto& bounds = player->getBounds();
    return bounds ? bounds.getEndSeek() : player->getDuration();
}

void ProjectController::rewindBackBySeconds(double seconds) {
    double result = player->getSeek() - seconds;
    if (result < 0) {
        result = 0;
    }
    player->setSeek(result);
}

std::u32string_view ProjectController::getLyricsLine(int index) const {
    return player->getFile().getLyrics().getLineTextAt(index);
}

int ProjectController::getLinesCount() const {
    return player->getFile().getLyrics().getLinesCount();
}

void ProjectController::setPlaybackBounds(const PlaybackBounds &bounds) {
    player->setBounds(bounds);
}

void ProjectController::setPlaybackBoundsUsingLineIndexes(int firstLineIndex, int lastLineIndex) {
    assert(firstLineIndex >= 0 && firstLineIndex <= lastLineIndex && lastLineIndex < getLinesCount());
    const Lyrics &lyrics = player->getFile().getLyrics();
    const auto& firstLine = lyrics.getLineAt(firstLineIndex);
    const auto& lastLine = lyrics.getLineAt(lastLineIndex);
    setPlaybackBounds(PlaybackBounds(firstLine.startSeek, lastLine.getEndSeek()));
}

bool ProjectController::hasPlaybackBounds() const {
    return player->getBounds();
}
