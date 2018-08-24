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
#include "PitchesCollector.h"
#include "VxFile.h"
#include "WorkspaceController.h"
#include "PianoDrawer.h"
#include "PlaybackBounds.h"
#include <memory>
#include <PlayingPitchSequence.h>

class WorkspaceDrawer : public WorkspaceController {
    typedef Drawer::Color Color;

    std::atomic<float> intervalWidth;
    std::atomic<float> intervalHeight;
    std::atomic<float> verticalOffset;
    std::atomic<float> horizontalOffset;
    std::atomic<double> intervalsPerSecond;
    std::atomic_bool running;
    std::atomic_int firstPitchIndex;
    std::atomic<float> summarizedGridHeight;
    std::atomic<float> verticalScrollPosition;

    std::atomic<PlaybackBounds> playbackBounds;

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

    std::string tactNumbersFontFamily;

    float width = -1;
    float height = -1;
    float devicePixelRatio = -1;

    Drawer* drawer = nullptr;
    PitchesCollector* pitchesCollector = nullptr;
    PianoDrawer* pianoDrawer = nullptr;

    std::atomic<const VxFile*> vxFile;

    std::atomic<double> frameTime;

    void iterateHorizontalIntervals(const std::function<void(float x, bool isBeat)>& func) const;

    void drawHorizontalLine(float y, const Color& color) const;
    void drawVerticalLine(float x, const Color& color) const;
    void drawVerticalGrid() const;
    void drawHorizontalGrid() const;
    void drawPitch(float x, float y, float width) const;
    void drawPitches() const;
    void drawPitchesGraph() const;
    void drawBoundsIfNeed() const;
    void drawYardStick() const;
    void drawYardStickDot(float x, float y) const;
    void drawSecondPlayHead() const;

    double getPitchGraphDuration() const;
    double getIntervalDuration() const;

    std::function<void()> onUpdateRequested;

    Pitch getFirstPitch() const;
    float getGridTranslation() const;
    float getMaximumGridTranslation() const;

    float getWorkspaceDuration() const;
    float getWorkspaceSeek() const;

    float durationToWidth(double duration) const;

    bool getBoundsStartXAndWidth(const PlaybackBounds& bounds, float* startX, float* width) const;
public:
    Drawer::Image* playHeadImage = nullptr;

    static constexpr float YARD_STICK_HEIGHT = 22;
    static constexpr int PIANO_WIDTH = 67;

    WorkspaceDrawer(Drawer *drawer, const std::function<void()>& onUpdateRequested);
    ~WorkspaceDrawer();

    void setOnUpdateRequested(const std::function<void()> &onUpdateRequested);

    void resize(float width, float height, float devicePixelRatio);
    void draw();

    float getIntervalWidth() const;
    void setIntervalWidth(float intervalWidth) override;
    float getIntervalHeight() const;
    void setIntervalHeight(float intervalHeight) override;
    float getVerticalOffset() const;
    void setVerticalOffset(float verticalOffset) override;
    float getHorizontalOffset() const override;
    void setHorizontalOffset(float horizontalOffset) override;

    double getIntervalsPerSecond() const override;
    void setIntervalsPerSecond(double intervalsPerSecond) override;

    float getSummarizedGridHeight() const;
    void setSummarizedGridHeight(float summarizedGridHeight) override;

    float getVerticalScrollPosition() const;
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

    void setTactNumbersFontFamily(const std::string &tactNumbersFontFamily);

    PitchesCollector *getPitchesCollector() const;
    void setPitchesCollector(PitchesCollector *pitchesCollector) override;

    // Warning: VxFile should not be changed from outside
    void setVxFile(const VxFile* vxFile) override;
    int getDistanceFromFirstPitch(const Pitch &pitch) const;

    void update() override;

    float getGridHeight() const;
    static float getGridHeight(float workspaceHeight);

    void setDetectedPitch(const Pitch &detectedPitch) override;
    void setPitchSequence(PlayingPitchSequence *pitchSequence) override;

    const PlaybackBounds &getPlaybackBounds() const;
    void setPlaybackBounds(const PlaybackBounds &playbackBounds) override;

    static void getPlayHeadImageSize(float summaryHeight, float devicePixelRatio, int* outW, int* outH);
    void setPlayHeadImage(Drawer::Image* image);

    void onMouseMove(float x);
    void onMouseClick(float x);
};


#endif //VOCALTRAINER_WORKSPACEDRAWER_H
