//
// Created by Semyon Tikhonenko on 5/26/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <nanovg/nanovg.h>
#define NANOVG_GL2_IMPLEMENTATION
#include <nanovg/nanovg_gl.h>

#include "WorkspaceDrawer.h"
#include <assert.h>
#include "CountAssert.h"
#include "Pitch.h"
#include "TimeUtils.h"
#include <iostream>

using namespace CppUtils;

constexpr int BEATS_IN_TACT = 4;

void WorkspaceDrawer::resize(float width, float height, float devicePixelRatio) {
    assert(devicePixelRatio > 0);
    assert(width >= 0 && height >= 0);
    this->devicePixelRatio = devicePixelRatio;
    this->width = width;
    this->height = height;

    if (!ctx) {
        ctx = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    }
}

void WorkspaceDrawer::draw() {
    assert(ctx && "call resize before draw");

    assert(intervalWidth >= 0);
    assert(intervalHeight >= 0);

    static double frameTime = 0;
    double now = TimeUtils::NowInSeconds();
    if (frameTime != 0) {
        double frameDuration = now - frameTime;
        if (speed != 0) {
            horizontalOffset = horizontalOffset + frameDuration * speed;
        }
    }
    frameTime = now;

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    nvgBeginFrame(ctx, width, height, devicePixelRatio);
    drawVerticalGrid();
    drawHorizontalGrid();
    nvgEndFrame(ctx);
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

void WorkspaceDrawer::drawVerticalGrid() const {
    int index = 1;
    float offset = fmod(horizontalOffset, intervalWidth * BEATS_IN_TACT);
    for (float x = intervalWidth - offset; x < width + offset; x += intervalWidth, index++) {
        nvgBeginPath(ctx);
        nvgMoveTo(ctx, x * sizeMultiplier, 0);
        nvgStrokeWidth(ctx, sizeMultiplier);
        nvgLineTo(ctx, x * sizeMultiplier, height * sizeMultiplier);
        bool isBeat = index % BEATS_IN_TACT != 0;
        nvgStrokeColor(ctx, isBeat ? gridColor : accentGridColor);
        nvgStroke(ctx);
    }
}

void WorkspaceDrawer::drawHorizontalGrid() const {
    int index = 1;
    float offset = fmod(verticalOffset, intervalHeight * Pitch::PITCHES_IN_OCTAVE);
    for (float y = height + intervalHeight + offset; y > -offset; y -= intervalHeight, index++) {
        nvgBeginPath(ctx);
        nvgMoveTo(ctx, 0, y * sizeMultiplier);
        nvgLineTo(ctx, width * sizeMultiplier, y * sizeMultiplier);
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

WorkspaceDrawer::WorkspaceDrawer() :
        intervalWidth(-1),
        intervalHeight(-1),
        verticalOffset(0),
        horizontalOffset(0),
        sizeMultiplier(1),
        speed(0)
{

}

WorkspaceDrawer::~WorkspaceDrawer() {
    if (ctx) {
        nvgDeleteGL2(ctx);
    }
}

float WorkspaceDrawer::getSizeMultiplier() const {
    return sizeMultiplier;
}

void WorkspaceDrawer::setSizeMultiplier(float sizeMultiplier) {
    assert(sizeMultiplier > 0);
    this->sizeMultiplier = sizeMultiplier;
}

double WorkspaceDrawer::getSpeed() const {
    return speed;
}

void WorkspaceDrawer::setSpeed(double speed) {
    this->speed = speed;
}
