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

class WorkspaceDrawer {
    typedef Drawer::Color Color;

    std::atomic<float> intervalWidth;
    std::atomic<float> intervalHeight;
    std::atomic<float> verticalOffset;
    std::atomic<float> horizontalOffset;
    std::atomic<double> intervalsPerSecond;
    int firstPitchPerfectFrequencyIndex;

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
    const VxFile* vxFile = nullptr;

    std::atomic<double> frameTime;

    void drawHorizontalLine(float y, const Color& color) const;
    void drawVerticalGrid() const;
    void drawHorizontalGrid() const;
    void drawPitch(float x, float y, float width) const;
    void drawPitches() const;
    void drawPitchesGraph() const;

    double getPitchGraphDuration() const;
    double getIntervalDuration() const;
public:
    WorkspaceDrawer();
    ~WorkspaceDrawer();
    void resize(float width, float height, float devicePixelRatio);
    void draw();

    float getIntervalWidth() const;
    void setIntervalWidth(float intervalWidth);
    float getIntervalHeight() const;
    void setIntervalHeight(float intervalHeight);
    float getVerticalOffset() const;
    void setVerticalOffset(float verticalOffset);
    float getHorizontalOffset() const;
    void setHorizontalOffset(float horizontalOffset);

    double getIntervalsPerSecond() const;
    void setIntervalsPerSecond(double intervalsPerSecond);
    int getFirstPitchPerfectFrequencyIndex() const;
    void setFirstPitchPerfectFrequencyIndex(int firstPitchPerfectFrequencyIndex);

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
    void setPitchesCollector(PitchesCollector *pitchesCollector);

    // Should be called on a render thread only, WorkspaceDrawer takes ownership,
    // usually you should make a copy before assigning
    void setVxFile(const VxFile* vxFile);
    int getDistanceFromFirstPitch(const Pitch &pitch) const;
};


#endif //VOCALTRAINER_WORKSPACEDRAWER_H
