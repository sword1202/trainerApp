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

NvgOpenGLDrawer::NvgOpenGLDrawer() {
    ctx = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
}

NvgOpenGLDrawer::~NvgOpenGLDrawer() {
    nvgDeleteGL2(ctx);
}

void NvgOpenGLDrawer::beginFrame(float width, float height, float devicePixelRatio) {
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
    nvgStrokeColor(ctx, nvgRGBA(color[0], color[1], color[2], color[3]));
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
