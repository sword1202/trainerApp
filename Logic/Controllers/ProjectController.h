//
// Created by Semyon Tykhonenko on 2019-07-12.
//

#ifndef TEXTIMAGESGENERATOR_PROJECTCONTROLLER_H
#define TEXTIMAGESGENERATOR_PROJECTCONTROLLER_H

#include <string>
#include "WorkspaceController.h"
#include "VocalTrainerFilePlayer.h"
#include "AudioInputManager.h"
#include "Timer.h"
#include "Rewinder.h"
#include "Point.h"
#include "Tonality.h"
#include "SingingCompletionFlow.h"
#include "NotImplementedAssert.h"

class ProjectControllerDelegate {
public:
    virtual inline void updateAudioLevel(double level) {}
    virtual inline void updateSeek(double seek) {}
    virtual inline void onPlaybackStarted() {}
    virtual inline void onPlaybackStopped() {}
    virtual inline void updateLyricsLines(const LyricsDisplayedLinesProvider* linesProvider) {}
    virtual inline void updateLyricsSelection(const LyricsPlayer::Selection& selection) {}
    virtual inline void updateLyricsVisibilityChanged(bool showLyrics) {}
    virtual inline void onMetronomeEnabledChanged(bool enabled) {}
    virtual inline void onTracksVisibilityChanged(bool value) {}
    virtual inline void updateVocalPianoVolume(float volume) {}
    virtual inline void updateInstrumentalVolume(float volume) {}
    virtual inline void updateVocalVolume(float volume) {}
    virtual inline void updateInputSensitivity(float value) {}
    virtual inline void updateZoom(float value) {}
    virtual inline void updateTonality(int shift) {}
    virtual inline void updateTempoFactor(double tempoFactor) {}
    virtual inline void updateEndSeek(double endSeek) {}
    virtual inline void onRewindStatusChanged(bool rewindRunning, bool backward) {}
    virtual inline void showSingingCompletionFlow(SingingCompletionFlow* songCompletionFlow) {}
    virtual inline void hideSingingCompletionFlow() {}
    virtual inline void startListeningToRecording(MvxFile* recording) { NOT_IMPLEMENTED_ASSERT }
    virtual std::shared_ptr<std::ostream> createStreamToSaveRecording(const VocalTrainerFile* recording) {
        return nullptr;
    }
};

class ProjectController : CppUtils::DestructorQueue, public WorkspaceControllerDelegate, private SingingCompletionFlow,
                          private CppUtils::OnThreadExecutor {
    WorkspaceController* workspaceController = nullptr;
    WorkspaceColorScheme workspaceColorScheme;
    ProjectControllerDelegate* delegate = nullptr;
    VocalTrainerFilePlayer* player = nullptr;
    VocalTrainerFile* source = nullptr;
    AudioInputManager* audioInputManager = nullptr;
    bool lyricsVisible = true;
    Rewinder* rewinder = nullptr;
    mutable std::vector<Lyrics::Section> sections;

    void onStopPlaybackRequested();
    void updateWorkspaceSeek(double seek);

    void play();

    MvxFile* generateRecording() const;

    void tryAgain() override;
    void save() override;
    void listen() override;
public:
    explicit ProjectController(ProjectControllerDelegate* delegate);

    // Public interface
    void setWorkspaceController(WorkspaceController *workspaceController);

    const std::string& getArtistNameUtf8();
    const std::string& getSongTitleUtf8();

    const Tonality& getOriginalTonality() const;

    void togglePlay();
    void stop();
    bool isPlaying() const;
    bool isRecording() const;
    void toggleRewind(bool backward);

    void rewindBackBySeconds(double seconds);

    void goToBeginning();

    void setPlaybackProgress(double value);
    double convertSeekToPlaybackProgress(double seek) const;

    bool isBoundsSelectionEnabled() const;
    void setBoundsSelectionEnabled(bool boundsSelectionEnabled);

    void setPlaybackBounds(const PlaybackBounds& bounds);
    bool hasPlaybackBounds() const;
    void setPlaybackBoundsUsingLineIndexes(int firstLineIndex, int lastLineIndex);


    void setVocalVolume(float value);
    void setInputSensitivity(float value);
    void setVocalPianoVolume(float value);
    void setInstrumentalVolume(float value);

    float getVocalVolume() const;
    float getInputSensitivity() const;
    float getVocalPianoVolume() const;
    float getInstrumentalVolume() const;
    
    void setLyricsVisible(bool value);
    void setMetronomeEnabled(bool value);
    void setTracksVisible(bool value);

    void setPitchShift(int value);
    void setTempoFactor(double value);

    bool isLyricsVisible() const;
    bool isTracksVisible() const;
    bool isMetronomeEnabled() const;

    void setZoom(float zoom);
    void setZoom(float zoom, const CppUtils::PointF& intoPoint);
    float getZoom() const;
    float getMinZoom() const;
    float getMaxZoom() const;

    double getBeatsPerMinute() const;
    double getOriginalBeatsPerMinute() const;
    double getEndSeek() const;

    float getVerticalScrollPosition() const;
    void setVerticalScrollPosition(float position);

    const std::vector<Lyrics::Section>& getLyricsSections() const;
    std::u32string_view getLyricsLine(int index) const;
    int getLinesCount() const;

    // Delegates
    void onPlaybackBoundsChangedByUserEvent(const PlaybackBounds &newBounds) override;
    void onSeekChangedByUserEvent(float newSeek) override;

    bool hasPlaybackSource() const;
    void setPlaybackSource(VocalTrainerFile* file);
    void setPlaybackSource(const char* filePath);

    std::vector<float> getRecordingPreview(int numberOfSamples) const;

    void setWorkspaceColors(WorkspaceColorScheme colorScheme);

    void updateDelegate();

    ~ProjectController();
};


#endif //TEXTIMAGESGENERATOR_PROJECTCONTROLLER_H
