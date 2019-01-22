//
// Created by Semyon Tikhonenko on 5/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_WORKSPACEDRAWER_H
#define VOCALTRAINER_WORKSPACEDRAWER_H

#include <atomic>
#include <nanovg/nanovg.h>
#include "Drawer.h"
#include <array>
#include "PitchesCollection.h"
#include "VxFile.h"
#include "WorkspaceController.h"
#include "PianoDrawer.h"
#include "PlaybackBounds.h"
#include <memory>
#include "PlayingPitchSequence.h"
#include "ScrollBar.h"
#include <thread>

class WorkspaceDrawer : public WorkspaceController {
    typedef Drawer::Color Color;

#ifndef NDEBUG
    std::thread::id threadId;
    bool checkExecutedOnRenderingThread();
#endif

    std::atomic<float> intervalWidth;
    std::atomic<float> intervalHeight;
    std::atomic<float> verticalOffset;
    std::atomic<float> horizontalOffset;
    // the same as beatsPerSecond
    std::atomic<double> intervalsPerSecond;
    std::atomic<double> totalDurationInSeconds;
    std::atomic_bool running;
    std::atomic_int firstPitchIndex;
    std::atomic_int lastPitchIndex;

    std::atomic<PlaybackBounds> playbackBounds;

    bool recording = false;

    float sizeMultiplier;
    float pitchRadius = 0;

    Color gridColor;
    Color accentGridColor;
    Color pitchGraphColor;
    Color pitchColor;
    Color borderLineColor;
    Color yardStickDotAndTextColor;
    Color boundsColor;
    Color playHeadColor;
    Color instrumentalTrackColor;
    Color trackButtonColor;
    Color pianoTrackColor;
    Color pianoTrackShadowColor;
    Color pianoTrackPitchesColor;

    float width = -1;
    float height = -1;
    float devicePixelRatio = -1;

    Drawer* drawer = nullptr;
    MouseEventsReceiver* mouseEventsReceiver;
    const PitchesCollection* pitchesCollection = nullptr;
    PianoDrawer* pianoDrawer = nullptr;

    Drawer::Image* playHeadTriangleImage = nullptr;
    Drawer::Image* clockImage = nullptr;

    std::atomic<const VxFile*> vxFile;

    std::atomic<double> frameTime;
    std::atomic<float> firstPlayHeadPosition, secondPlayHeadPosition;

    std::vector<float> pitchesFrequencies;
    std::vector<double> pitchesTimes;

    std::vector<short> instrumentalTrackSamples;
    Drawer::Image* instrumentalTrackImage = nullptr;
    Drawer::Image* instrumentalTrackButtonImage = nullptr;
    Drawer::Image* pianoTrackButtonImage = nullptr;

    std::atomic_bool drawTracks;

    std::atomic<float> zoom;

    ScrollBar horizontalScrollBar;
    ScrollBar verticalScrollBar;
    std::function<void(float)> seekUpdatedInsideListener;

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
    void drawPianoTrack();
    void drawPianoTrackButton();
    void drawScrollBars();

    double getSingingPitchGraphDuration() const;
    double getIntervalDuration() const;

    std::function<void()> onUpdateRequested;

    Pitch getFirstPitch() const;
    float getGridTranslation() const;
    float getMaximumGridTranslation() const;

    float getWorkspaceDuration() const;

    float durationToWidth(double duration) const;

    bool getBoundsStartXAndWidth(const PlaybackBounds& bounds, float* startX, float* width) const;
    void updateHorizontalScrollBarPageSize();
    void updateHorizontalScrollBarPagePosition();
public:

    static constexpr int PIANO_WIDTH = 67;
    static constexpr float PLAYHEAD_TRIANGLE_WIDTH = 12.0f;
    static constexpr float PLAYHEAD_TRIANGLE_HEIGHT = 12.0f;
    static constexpr float CLOCK_WIDTH = 42.f;
    static constexpr float CLOCK_HEIGHT = 22.f;
    static constexpr float YARD_STICK_HEIGHT = CLOCK_HEIGHT  + PLAYHEAD_TRIANGLE_HEIGHT / 2;
    static constexpr int YARD_STICK_FONT_SIZE = 11;
    static constexpr int CLOCK_FONT_SIZE = 11;
    static constexpr float MIN_ZOOM = 1.0;
    static constexpr float MAX_ZOOM = 2.0;

    WorkspaceDrawer(Drawer *drawer, MouseEventsReceiver *mouseEventsReceiver,
                        const std::function<void()> &onUpdateRequested);
    ~WorkspaceDrawer();

    void setOnUpdateRequested(const std::function<void()> &onUpdateRequested);

    void resize(float width, float height, float devicePixelRatio);
    void draw();

    float getHorizontalOffset() const override;

    double getIntervalsPerSecond() const override;
    void setIntervalsPerSecond(double intervalsPerSecond) override;

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

    // Warning: VxFile should not be changed from outside
    void setVxFile(const VxFile* vxFile) override;
    int getDistanceFromFirstPitch(const Pitch &pitch) const;

    void update() override;

    float getVisibleGridHeight() const override;
    float getVisibleGridWidth() const override;
    float getSummarizedGridWidth() const override;

    void setDetectedPitch(const Pitch &detectedPitch) override;
    void setPitchSequence(PlayingPitchSequence *pitchSequence) override;

    const PlaybackBounds &getPlaybackBounds() const;
    void setPlaybackBounds(const PlaybackBounds &playbackBounds) override;

    void setPlayHeadTriangleImage(Drawer::Image *image);
    void setClockImage(Drawer::Image *clockImage);

    float getWorkspaceSeek() const override;
    float getGridBeginXPosition() const;
    float getSeekFromXPositionOnWorkspace(float x) override;

    float getPlayHeadXPosition(int playHeadIndex) override;

    void setRecording(bool recording) override;

    void generateInstrumentalTrackSamplesImage(float width);
    void setInstrumentalTrackSamples(const std::vector<short> &instrumentalTrackSamples) override;
    void setInstrumentalTrackButtonImage(Drawer::Image *instrumentalTrackButtonImage);

    void setPianoTrackButtonImage(Drawer::Image *pianoTrackButtonImage);

    void setDrawTracks(bool value) override;

    float getZoom() const override;
    void setZoom(float zoom) override;

    void updateSeek(float seek) override;

    void setSeekUpdatedInsideListener(const std::function<void(float)> &listener) override;
};


#endif //VOCALTRAINER_WORKSPACEDRAWER_H
