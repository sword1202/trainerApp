//
// Created by Semyon Tikhonenko on 5/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <nanovg.h>
#define NANOVG_GL2_IMPLEMENTATION
#include <nanovg_gl.h>

#include "WorkspaceDrawer.h"
#include <assert.h>
#include "CountAssert.h"
#include "Pitch.h"

constexpr int BEATS_IN_TACT = 4;

void WorkspaceDrawer::draw(float width, float height, float devicePixelRatio) {
    assert(devicePixelRatio > 0);
    assert(width >= 0 && height >= 0);
    assert(intervalWidth >= 0);
    assert(intervalHeight >= 0);
    assert(verticalOffset >= 0);
    assert(horizontalOffset >= 0);

    this->width = width;
    this->height = height;
    this->devicePixelRatio = devicePixelRatio;

    ctx = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    nvgBeginFrame(ctx, width, height, devicePixelRatio);
    drawVerticalGrid();
    drawHorizontalGrid();
    nvgEndFrame(ctx);
    nvgDeleteGL2(ctx);
}

float WorkspaceDrawer::getIntervalWidth() const {
    return intervalWidth;
}

void WorkspaceDrawer::setIntervalWidth(float intervalWidth) {
    this->intervalWidth = intervalWidth;
}

float WorkspaceDrawer::getIntervalHeight() const {
    return intervalHeight;
}

void WorkspaceDrawer::setIntervalHeight(float intervalHeight) {
    this->intervalHeight = intervalHeight;
}

float WorkspaceDrawer::getVerticalOffset() const {
    return verticalOffset;
}

void WorkspaceDrawer::setVerticalOffset(float verticalOffset) {
    this->verticalOffset = verticalOffset;
}

float WorkspaceDrawer::getHorizontalOffset() const {
    return horizontalOffset;
}

void WorkspaceDrawer::setHorizontalOffset(float horizontalOffset) {
    this->horizontalOffset = horizontalOffset;
}

void WorkspaceDrawer::drawVerticalGrid() {
    int index = 1;
    for (float x = intervalWidth - horizontalOffset; x < width + horizontalOffset; x += intervalWidth, index++) {
        nvgBeginPath(ctx);
        nvgMoveTo(ctx, x * devicePixelRatio, 0);
        nvgStrokeWidth(ctx, devicePixelRatio);
        nvgLineTo(ctx, x * devicePixelRatio, height * devicePixelRatio);
        bool isBeat = index % BEATS_IN_TACT != 0;
        nvgStrokeColor(ctx, isBeat ? gridColor : accentGridColor);
        nvgStroke(ctx);
    }
}

void WorkspaceDrawer::drawHorizontalGrid() {
    int index = 1;
    for (float y = intervalHeight - verticalOffset; y < height + verticalOffset; y += intervalHeight, index++) {
        nvgBeginPath(ctx);
        nvgMoveTo(ctx, 0, y * devicePixelRatio);
        nvgLineTo(ctx, width * devicePixelRatio, y * devicePixelRatio);
        bool isOctaveBegin = index % Pitch::PITCHES_IN_OCTAVE == 0;
        nvgStrokeColor(ctx, isOctaveBegin ? accentGridColor : gridColor);
        nvgStroke(ctx);
    }
}

const NVGcolor &WorkspaceDrawer::getGridColor() const {
    return gridColor;
}

void WorkspaceDrawer::setGridColor(const NVGcolor &gridColor) {
    // Should be called only once before rendering to avoid synchronization issues
    CountAssert(1);
    this->gridColor = gridColor;
}

const NVGcolor &WorkspaceDrawer::getAccentGridColor() const {
    return accentGridColor;
}

void WorkspaceDrawer::setAccentGridColor(const NVGcolor &accentGridColor) {
    // Should be called only once before rendering to avoid synchronization issues
    CountAssert(1);
    this->accentGridColor = accentGridColor;
}

WorkspaceDrawer::WorkspaceDrawer() : intervalWidth(-1),
        intervalHeight(-1),
        verticalOffset(-1),
        horizontalOffset(-1) {
}

WorkspaceDrawer::~WorkspaceDrawer() {
    if (ctx) {
        //nvgDeleteGL2(ctx);
    }
}
