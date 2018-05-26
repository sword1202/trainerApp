//
// Created by Semyon Tikhonenko on 5/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_WORKSPACEDRAWER_H
#define VOCALTRAINER_WORKSPACEDRAWER_H

#include <atomic>
#include <nanovg.h>

class WorkspaceDrawer {
    std::atomic<float> intervalWidth;
    std::atomic<float> intervalHeight;
    std::atomic<float> verticalOffset;
    std::atomic<float> horizontalOffset;

    float width = -1;
    float height = -1;
    float devicePixelRatio = -1;

    NVGcontext* ctx;

    void drawVerticalGrid();

    NVGcolor gridColor;
    NVGcolor accentGridColor;
public:
    WorkspaceDrawer();
    void draw(float width, float height, float devicePixelRatio);

    float getIntervalWidth() const;
    void setIntervalWidth(float intervalWidth);
    float getIntervalHeight() const;
    void setIntervalHeight(float intervalHeight);
    float getVerticalOffset() const;
    void setVerticalOffset(float verticalOffset);
    float getHorizontalOffset() const;
    void setHorizontalOffset(float horizontalOffset);

    const NVGcolor &getGridColor() const;
    void setGridColor(const NVGcolor &gridColor);
    const NVGcolor &getAccentGridColor() const;
    void setAccentGridColor(const NVGcolor &accentGridColor);
};


#endif //VOCALTRAINER_WORKSPACEDRAWER_H
