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

class WorkspaceDrawer {
    typedef Drawer::Color Color;

    std::atomic<float> intervalWidth;
    std::atomic<float> intervalHeight;
    std::atomic<float> verticalOffset;
    std::atomic<float> horizontalOffset;
    std::atomic<float> sizeMultiplier;
    std::atomic<double> instervalsPerSecond;

    Color gridColor;
    Color accentGridColor;

    float width = -1;
    float height = -1;
    float devicePixelRatio = -1;

    Drawer* drawer = nullptr;

    void drawHorizontalLine(float y, const Color& color) const;
    void drawVerticalGrid() const;
    void drawHorizontalGrid() const;
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

    float getSizeMultiplier() const;
    void setSizeMultiplier(float sizeMultiplier);

    const Color &getGridColor() const;
    void setGridColor(const Color& color);
    const Color &getAccentGridColor() const;
    void setAccentGridColor(const Color& color);
};


#endif //VOCALTRAINER_WORKSPACEDRAWER_H
