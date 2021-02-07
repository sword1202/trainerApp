//
// Created by Semyon Tikhonenko on 5/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_WORKSPACEDRAWER_H
#define VOCALTRAINER_WORKSPACEDRAWER_H

#include "Drawer.h"
#include <array>
#include "PitchesCollection.h"
#include "VocalPart.h"
#include "WorkspaceController.h"
#include "PianoDrawer.h"
#include "PlaybackBounds.h"
#include <memory>
#include "PlayingPitchSequence.h"
#include "ScrollBar.h"
#include "WorkspaceDrawerResourcesProvider.h"
#include "BoundsSelectionController.h"
#include "MouseClickChecker.h"
#include <thread>

class BoundsSelectionController;

class WorkspaceDrawer : public WorkspaceController {
    typedef Drawer::Color Color;

#ifndef NDEBUG
    std::thread::id threadId;
    bool checkExecutedOnRenderingThread();
#endif

    float intervalWidth;
    float intervalHeight;
    float horizontalOffset;
    double beatsPerSecond;
    double totalDurationInSeconds;
    bool running;
    std::atomic_int firstPitchIndex;
    std::atomic_int lastPitchIndex;

    PlaybackBounds playbackBounds;

    bool recording = false;

    float sizeMultiplier;
    float pitchRadius = 0;

    Color gridColor;
    Color accentGridColor;
    Color pitchGraphColor;
    Color pitchColor;
    Color borderLineColor;
    Color boundsColor;
    Color playHeadColor;
    Color instrumentalTrackColor;
    Color trackButtonColor;
    Color pianoTrackColor;
    Color pianoTrackShadowColor;
    Color pianoTrackPitchesColor;
    Color playbackMarksLineColor;
    Color endingColor;
    Color playbackMarksRectColor;

    int yardStickFontSize = 11;
    Drawer::FontStyle yardStickFontStyle = Drawer::NORMAL;
    int clockFontSize = 11;
    Drawer::FontStyle clockFontStyle = Drawer::NORMAL;

    float width = -1;
    float height = -1;
    float devicePixelRatio = -1;

    Drawer* drawer = nullptr;
    MouseEventsReceiver* mouseEventsReceiver;
    MouseClickChecker mouseClickChecker;
    const PitchesCollection* pitchesCollection = nullptr;
    PianoDrawer* pianoDrawer = nullptr;

    Drawer::Image* playHeadTriangleImage = nullptr;
    Drawer::Image* clockImage = nullptr;

    const VocalPart* vocalPart;

    double frameTime;
    float firstPlayHeadPosition, secondPlayHeadPosition;

    std::vector<float> pitchesFrequencies;
    std::vector<double> pitchesTimes;

    std::vector<short> instrumentalTrackSamples;
    Drawer::Image* instrumentalTrackImage = nullptr;
    Drawer::Image* instrumentalTrackButtonImage = nullptr;
    Drawer::Image* pianoTrackButtonImage = nullptr;

    bool willDrawTracks;

    float zoom;
    float minZoom;
    float maxZoom;

    // Scrollbars are not drawn for small screens
    ScrollBar horizontalScrollBar;
    ScrollBar verticalScrollBar;
    bool willDrawScrollbars;

    WorkspaceDrawerResourcesProvider* resourcesProvider;
    WorkspaceControllerDelegate* delegate = nullptr;
    BoundsSelectionController* boundsSelectionController = nullptr;

    void iterateHorizontalIntervals(const std::function<void(float x, bool isBeat)>& func) const;

    void drawHorizontalLine(float y, const Color& color) const;
    void drawVerticalLine(float x, const Color& color) const;
    void drawVerticalGrid() const;
    void drawHorizontalGrid() const;
    void drawPitch(float x, float y, float width) const;
    void drawPitches() const;
    void initGraphPitchesArrays(float workspaceSeek);
    void drawPitchesGraph();
    void drawBoundsIfNeed() const;
    void drawYardStick() const;
    void drawYardStickDot(float x, float y) const;
    void drawPlayHead(float x, float timeInSeconds);
    void drawFirstPlayHead();
    void drawSecondPlayHead();
    void drawInstrumentalTrack();
    void drawInstrumentalTrackButton();

    float drawPianoTrackAndCalculateHeight();
    void drawPianoTrackButton(float pianoTrackHeight);
    void drawScrollBars();
    void drawEnding();
    void drawTracks();
    void drawFps(float fps);

    double getSingingPitchGraphDuration() const;

    std::function<void()> onUpdateRequested;

    Pitch getFirstPitch() const;
    float getGridYTranslation() const;
    float getMaximumGridYTranslation() const;

    float getWorkspaceDuration() const;

    float durationToWidth(double duration) const;

    bool getBoundsStartXAndWidth(const PlaybackBounds& bounds, float* startX, float* width) const;
    void updateHorizontalScrollBarPageSize();
    void updateHorizontalScrollBarPagePosition();
    void updateZoom();

    void initImages();
    void captureClickEventsInTracksArea(float pianoTrackHeight);
public:

    static constexpr int PIANO_WIDTH = 67;
    static constexpr float PLAYHEAD_TRIANGLE_WIDTH = 12.0f;
    static constexpr float PLAYHEAD_TRIANGLE_HEIGHT = 12.0f;
    static constexpr float CLOCK_WIDTH = 42.f;
    static constexpr float CLOCK_HEIGHT = 22.f;
    static constexpr float YARD_STICK_HEIGHT = CLOCK_HEIGHT  + PLAYHEAD_TRIANGLE_HEIGHT / 2;
    static const Color YARD_STICK_DOT_AND_TEXT_COLOR;
    static constexpr int BEATS_IN_TACT = 4;

    WorkspaceDrawer(Drawer *drawer,
            MouseEventsReceiver *mouseEventsReceiver,
            WorkspaceDrawerResourcesProvider *resourcesProvider,
            bool drawScrollbars,
            const std::function<void()> &onUpdateRequested);
    ~WorkspaceDrawer();

    void setOnUpdateRequested(const std::function<void()> &onUpdateRequested);

    void resize(float width, float height, float devicePixelRatio);
    void draw();

    float getHorizontalOffset() const override;

    double getBeatsPerSecond() const override;
    void setBeatsPerSecond(double beatsPerSecond) override;
    double getBeatDuration() const;

    double getTotalDurationInSeconds() const;
    void setTotalDurationInSeconds(double totalDurationInSeconds) override;

    float getSummarizedGridHeight() const override;

    float getVerticalScrollPosition() const override;
    void setVerticalScrollPosition(float verticalScrollPosition) override;

    bool isRunning() const override;
    void setRunning(bool value) override;

    void setFirstVisiblePitch(const Pitch &firstPitch) override;

    float getSizeMultiplier() const;
    void setSizeMultiplier(float sizeMultiplier);

    float getPitchRadius() const;
    void setPitchRadius(float pitchRadius);

    const Color &getGridColor() const;
    void setGridColor(const Color& color);
    const Color &getAccentGridColor() const;
    void setAccentGridColor(const Color& color);
    const Color &getPitchGraphColor() const;
    void setPitchGraphColor(const Color &pitchGraphColor);
    const Color &getPitchColor() const;
    void setPitchColor(const Color &pitchColor);

    void setPitchesCollection(const PitchesCollection *pitchesCollection) override;

    // Warning: VocalPart should not be changed from outside
    void setVocalPart(const VocalPart *vocalPart) override;
    int getDistanceFromFirstPitch(const Pitch &pitch) const;

    void update() override;

    float getVisibleGridHeight() const override;
    float getVisibleGridWidth() const override;
    float getSummarizedPlayableGridWidth() const override;

    void setDetectedPitch(const Pitch &detectedPitch) override;
    void setPitchSequence(PlayingPitchSequence *pitchSequence) override;

    const PlaybackBounds &getPlaybackBounds() const;
    void setPlaybackBounds(const PlaybackBounds &playbackBounds) override;

    float getWorkspaceSeek() const override;
    float getGridBeginXPosition() const;
    float getGridBeginYPosition() const;
    float getZeroSeekGridOffset() const;
    float getSeekFromXPositionOnWorkspace(float x) const;

    float getPlayHeadXPosition(int playHeadIndex);

    void setRecording(bool recording) override;

    void generateInstrumentalTrackSamplesImage(float width);
    void setInstrumentalTrackSamples(const std::vector<short> &instrumentalTrackSamples) override;

    void setDrawTracks(bool value) override;

    bool shouldDrawTracks() override;

    float getZoom() const override;
    void setZoom(float zoom) override;
    void setZoom(float zoom, const CppUtils::PointF& intoPoint) override;

    float getMinZoom() const override;
    void setMinZoom(float minZoom);
    float getMaxZoom() const override;
    void setMaxZoom(float maxZoom);

    void updateSeek(float seek) override;

    void setDelegate(WorkspaceControllerDelegate *delegate) override;
    void setBoundsSelectionEnabled(bool boundsSelectionEnabled) override;
    bool isBoundsSelectionEnabled() const override;
};


#endif //VOCALTRAINER_WORKSPACEDRAWER_H
