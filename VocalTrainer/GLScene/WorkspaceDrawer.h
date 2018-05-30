//
// Created by Semyon Tikhonenko on 5/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_WORKSPACEDRAWER_H
#define VOCALTRAINER_WORKSPACEDRAWER_H

#include <atomic>
#include <nanovg/nanovg.h>

class WorkspaceDrawer {
    std::atomic<float> intervalWidth;
    std::atomic<float> intervalHeight;
    std::atomic<float> verticalOffset;
    std::atomic<float> horizontalOffset;
    std::atomic<float> sizeMultiplier;
    std::atomic<double> speed;

    NVGcolor gridColor;
    NVGcolor accentGridColor;

    float width = -1;
    float height = -1;
    float devicePixelRatio = -1;

    NVGcontext* ctx = nullptr;

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

    double getSpeed() const;

    void setSpeed(double speed);

    float getSizeMultiplier() const;

    void setSizeMultiplier(float sizeMultiplier);

    const NVGcolor &getGridColor() const;
    void setGridColor(const NVGcolor &gridColor);
    const NVGcolor &getAccentGridColor() const;
    void setAccentGridColor(const NVGcolor &accentGridColor);
};


#endif //VOCALTRAINER_WORKSPACEDRAWER_H
