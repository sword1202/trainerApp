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
#include <memory>

class WorkspaceDrawer : public WorkspaceController {
    typedef Drawer::Color Color;

    std::atomic<float> intervalWidth;
    std::atomic<float> intervalHeight;
    std::atomic<float> verticalOffset;
    std::atomic<float> horizontalOffset;
    std::atomic<double> intervalsPerSecond;
    std::atomic_bool running;
    Pitch firstPitch;

    float sizeMultiplier;
    float pitchRadius = 0;

    Color gridColor;
    Color accentGridColor;
    Color pitchGraphColor;
    Color pitchColor;

    float width = -1;
    float height = -1;
    float devicePixelRatio = -1;

    Drawer* drawer = nullptr;
    PitchesCollector* pitchesCollector = nullptr;
    std::atomic<const VxFile*> vxFile;

    std::atomic<double> frameTime;

    void drawHorizontalLine(float y, const Color& color) const;
    void drawVerticalGrid() const;
    void drawHorizontalGrid() const;
    void drawPitch(float x, float y, float width) const;
    void drawPitches() const;
    void drawPitchesGraph() const;

    double getPitchGraphDuration() const;
    double getIntervalDuration() const;

    std::function<void()> onUpdateRequested;
    std::function<void(float)> onHorizontalOffsetChanged;
public:
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

    PitchesCollector *getPitchesCollector() const;
    void setPitchesCollector(PitchesCollector *pitchesCollector) override;

    // Warning: VxFile should not be changed from outside
    void setVxFile(const VxFile* vxFile) override;
    int getDistanceFromFirstPitch(const Pitch &pitch) const;

    void update() override;

    void setHorizontalOffsetChangedListener(const std::function<void(float)> &listener) override;
};


#endif //VOCALTRAINER_WORKSPACEDRAWER_H
