//
// Created by Semyon Tikhonenko on 6/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_NVGOPENGLDRAWER_H
#define VOCALTRAINER_NVGOPENGLDRAWER_H

#include "Drawer.h"
#include <nanovg/nanovg.h>

class NvgOpenGLDrawer : public Drawer {
    NVGcontext* ctx = nullptr;

    float brushX = 0;
    float brushY = 0;
public:
    NvgOpenGLDrawer();
    virtual ~NvgOpenGLDrawer();

    void beginFrame(float width, float height, float devicePixelRatio) override;
    void endFrame() override;
    void moveTo(float x, float y) override;
    void lineTo(float x, float y) override;
    void setStrokeColor(const Color& color) override;
    void setStrokeWidth(float strokeWidth) override;
    void stroke() override;
    void beginPath() override;
    void clear() override;

    void moveBrush(float x, float y) override;

    void setBrushX(float x) override;

    void setBrushY(float y) override;

    float getBrushX() override;

    float getBrushY() override;
};


#endif //VOCALTRAINER_NVGOPENGLDRAWER_H
