//
// Created by Semyon Tikhonenko on 6/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "NvgDrawer.h"
#include <nanovg/nanovg.h>

#include <nanovg/nanovg_mtl.h>

#include <assert.h>

static NVGcolor toNvgColor(const Drawer::Color& color) {
    return nvgRGBA(color[0], color[1], color[2], color[3]);
}

#ifdef __APPLE__

void NvgDrawer::clear() {
    mnvgClearWithColor(ctx, nvgRGBA(255, 255, 255, 255));
}

NvgDrawer::NvgDrawer(void* layer) {
    ctx = nvgCreateMTL(layer, NVG_ANTIALIAS);
}

NvgDrawer::~NvgDrawer() {
    nvgDeleteMTL(ctx);
}

#else

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#define NANOVG_GL2_IMPLEMENTATION
#include <nanovg/nanovg_gl.h>

void NvgDrawer::clear() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

NvgDrawer::NvgDrawer() {
    ctx = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
}

NvgDrawer::~NvgDrawer() {
    nvgDeleteGL2(ctx);
}
#endif

void NvgDrawer::beginFrame(float width, float height, float devicePixelRatio) {
    translateX = 0;
    translateY = 0;
    nvgBeginFrame(ctx, width, height, devicePixelRatio);
}

void NvgDrawer::endFrame() {
    nvgEndFrame(ctx);
}

void NvgDrawer::moveTo(float x, float y) {
    nvgMoveTo(ctx, x, y);
}

void NvgDrawer::lineTo(float x, float y) {
    nvgLineTo(ctx, x, y);
}

void NvgDrawer::setStrokeColor(const Color& color) {
    nvgStrokeColor(ctx, toNvgColor(color));
}

void NvgDrawer::setStrokeWidth(float strokeWidth) {
    assert(strokeWidth > 0);
    nvgStrokeWidth(ctx, strokeWidth);
}

void NvgDrawer::stroke() {
    nvgStroke(ctx);
}

void NvgDrawer::beginPath() {
    nvgBeginPath(ctx);
}

void NvgDrawer::bezierCurveTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {
    nvgBezierTo(ctx, c1x, c1y, c2x, c2y, x, y);
}

void NvgDrawer::closePath() {
    nvgClosePath(ctx);
}

void NvgDrawer::fill() {
    nvgFill(ctx);
}

void NvgDrawer::setFillColor(const Drawer::Color &color) {
    nvgFillColor(ctx, toNvgColor(color));
}

void NvgDrawer::lineJoin(Drawer::LineJoin type) {
    int nvgValue;
    switch (type) {
        case BEVEL:
            nvgValue = NVG_BEVEL;
            break;
        case ROUND:
            nvgValue = NVG_ROUND;
            break;
        case MITER:
            nvgValue = NVG_MITER;
            break;
    }

    nvgLineJoin(ctx, nvgValue);
}

void NvgDrawer::rotate(float angle) {
    nvgRotate(ctx, angle);
}

void NvgDrawer::scale(float x, float y) {
    nvgScale(ctx, x, y);
}

void NvgDrawer::translate(float x, float y) {
    nvgTranslate(ctx, x, y);
    translateX += x;
    translateY += y;
}

float NvgDrawer::getTranslateX() {
    return translateX;
}

float NvgDrawer::getTranslateY() {
    return translateY;
}

void NvgDrawer::translateTo(float x, float y) {
    nvgTranslate(ctx, -translateX + x, -translateY + y);
    translateX = x;
    translateY = y;
}

void NvgDrawer::arcTo(float x1, float y1, float x2, float y2, float radius) {
    assert(radius >= 0);
    nvgArcTo(ctx, x1, y1, x2, y2, radius);
}

void NvgDrawer::rect(float x, float y, float w, float h) {
    nvgRect(ctx, x, y, w, h);
}

void NvgDrawer::fillRect(float x, float y, float w, float h) {
    nvgRect(ctx, x, y, w, h);
    nvgFill(ctx);
}
