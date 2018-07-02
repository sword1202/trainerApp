//
// Created by Semyon Tikhonenko on 6/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_NVGOPENGLDRAWER_H
#define VOCALTRAINER_NVGOPENGLDRAWER_H

#include "Drawer.h"
#include <nanovg/nanovg.h>

class NvgDrawer : public Drawer {
    NVGcontext* ctx = nullptr;

    float translateX = 0;
    float translateY = 0;
public:
#ifndef __APPLE__
    NvgDrawer();
#else
    NvgDrawer(void* layer);
#endif
    virtual ~NvgDrawer();

    void beginFrame(float width, float height, float devicePixelRatio) override;
    void endFrame() override;
    void moveTo(float x, float y) override;
    void lineTo(float x, float y) override;
    void arcTo(float x1, float y1, float x2, float y2, float radius) override;
    void setStrokeColor(const Color& color) override;
    void setFillColor(const Color& color) override;
    void setStrokeWidth(float strokeWidth) override;
    void stroke() override;
    void fill() override;
    void lineJoin(LineJoin type) override;

    void bezierCurveTo(float c1x, float c1y, float c2x, float c2y, float x, float y) override;
    void beginPath() override;
    void closePath() override;
    void rotate(float angle) override;
    void scale(float x, float y) override;
    void rect(float x, float y, float w, float h) override;
    void fillRect(float x, float y, float w, float h) override;

    void clear() override;

    void translate(float x, float y) override;
    float getTranslateX() override;
    float getTranslateY() override;
    void translateTo(float x, float y) override;
};


#endif //VOCALTRAINER_NVGOPENGLDRAWER_H
