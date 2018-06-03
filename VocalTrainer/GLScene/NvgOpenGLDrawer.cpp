//
// Created by Semyon Tikhonenko on 6/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "NvgOpenGLDrawer.h"

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <nanovg/nanovg.h>
#define NANOVG_GL2_IMPLEMENTATION
#include <nanovg/nanovg_gl.h>
#include <assert.h>

static NVGcolor toNvgColor(const Drawer::Color& color) {
    return nvgRGBA(color[0], color[1], color[2], color[3]);
}

NvgOpenGLDrawer::NvgOpenGLDrawer() {
    ctx = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
}

NvgOpenGLDrawer::~NvgOpenGLDrawer() {
    nvgDeleteGL2(ctx);
}

void NvgOpenGLDrawer::beginFrame(float width, float height, float devicePixelRatio) {
    translateX = 0;
    translateY = 0;
    nvgBeginFrame(ctx, width, height, devicePixelRatio);
}

void NvgOpenGLDrawer::endFrame() {
    nvgEndFrame(ctx);
}

void NvgOpenGLDrawer::moveTo(float x, float y) {
    nvgMoveTo(ctx, x, y);
}

void NvgOpenGLDrawer::lineTo(float x, float y) {
    nvgLineTo(ctx, x, y);
}

void NvgOpenGLDrawer::setStrokeColor(const Color& color) {
    nvgStrokeColor(ctx, toNvgColor(color));
}

void NvgOpenGLDrawer::setStrokeWidth(float strokeWidth) {
    assert(strokeWidth > 0);
    nvgStrokeWidth(ctx, strokeWidth);
}

void NvgOpenGLDrawer::stroke() {
    nvgStroke(ctx);
}

void NvgOpenGLDrawer::beginPath() {
    nvgBeginPath(ctx);
}

void NvgOpenGLDrawer::clear() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void NvgOpenGLDrawer::bezierCurveTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {
    nvgBezierTo(ctx, c1x, c1y, c2x, c2y, x, y);
}

void NvgOpenGLDrawer::closePath() {
    nvgClosePath(ctx);
}

void NvgOpenGLDrawer::fill() {
    nvgFill(ctx);
}

void NvgOpenGLDrawer::setFillColor(const Drawer::Color &color) {
    nvgFillColor(ctx, toNvgColor(color));
}

void NvgOpenGLDrawer::lineJoin(Drawer::LineJoin type) {
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

void NvgOpenGLDrawer::rotate(float angle) {
    nvgRotate(ctx, angle);
}

void NvgOpenGLDrawer::scale(float x, float y) {
    nvgScale(ctx, x, y);
}

void NvgOpenGLDrawer::translate(float x, float y) {
    nvgTranslate(ctx, x, y);
    translateX += x;
    translateY += y;
}

float NvgOpenGLDrawer::getTranslateX() {
    return translateX;
}

float NvgOpenGLDrawer::getTranslateY() {
    return translateY;
}

void NvgOpenGLDrawer::translateTo(float x, float y) {
    nvgTranslate(ctx, -translateX + x, -translateY + y);
    translateX = x;
    translateY = y;
}

void NvgOpenGLDrawer::arcTo(float x1, float y1, float x2, float y2, float radius) {
    assert(radius >= 0);
    nvgArcTo(ctx, x1, y1, x2, y2, radius);
}

void NvgOpenGLDrawer::rect(float x, float y, float w, float h) {
    nvgRect(ctx, x, y, w, h);
}
