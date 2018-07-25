//
// Created by Semyon Tikhonenko on 6/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "NvgDrawer.h"
#include <nanovg/nanovg.h>
#include <assert.h>

static NVGcolor toNvgColor(const Drawer::Color& color) {
    return nvgRGBA(color[0], color[1], color[2], color[3]);
}

#ifdef USE_METAL

#include <nanovg/nanovg_mtl.h>

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
#include <nanovg/fontstash.h>

void NvgDrawer::clear() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

NvgDrawer::NvgDrawer() {
    ctx = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
    setTextAlign(textAlign);
    setTextBaseline(textBaseline);
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

void NvgDrawer::roundedRectDifferentCorners(float x, float y, float w, float h, float radiusLeftTop,
        float radiusRightTop,
        float radiusBottomRight, float radiusBottomLeft) {
    nvgRoundedRectVarying(ctx, x, y, w, h, radiusLeftTop, radiusRightTop, radiusBottomRight, radiusBottomLeft);
}

void NvgDrawer::roundedRect(float x, float y, float w, float h, float r) {
    Drawer::roundedRect(x, y, w, h, r);
}

void NvgDrawer::quadraticCurveTo(float cpx, float cpy, float x, float y) {
    nvgQuadTo(ctx, cpx, cpy, x, y);
}

void NvgDrawer::fillText(const std::string &text, float x, float y) {
    if (nvgFindFont(ctx, fontFamily.data()) == FONS_INVALID){
        throw std::runtime_error("Font " + fontFamily + " was not registered, call drawer->registerFont before fillText call");
    }

    nvgText(ctx, x, y, text.data(), text.data() + text.size());
}

void NvgDrawer::setTextFont(const std::string &fontFamily, float fontSize) {
    Drawer::setTextFont(fontFamily, fontSize);
    nvgFontFace(ctx, fontFamily.data());
    nvgFontSize(ctx, fontSize);
}

static NVGalign toNVGAlign(Drawer::TextAlign align) {
    switch (align) {
        case Drawer::CENTER:
            return NVG_ALIGN_CENTER;
        case Drawer::LEFT:
            return NVG_ALIGN_LEFT;
        case Drawer::RIGHT:
            return NVG_ALIGN_RIGHT;
    }

    assert(false);
    return NVG_ALIGN_CENTER;
}

static NVGalign toNVGAlign(Drawer::TextBaseline align) {
    switch (align) {
        case Drawer::TOP:
            return NVG_ALIGN_TOP;
        case Drawer::BOTTOM:
            return NVG_ALIGN_BOTTOM;
        case Drawer::MIDDLE:
            return NVG_ALIGN_MIDDLE;
    }

    assert(false);
    return NVG_ALIGN_MIDDLE;
}

void NvgDrawer::setTextAlign(Drawer::TextAlign align) {
    Drawer::setTextAlign(align);
    nvgTextAlign(ctx, toNVGAlign(align) | toNVGAlign(textBaseline));
}

void NvgDrawer::setTextBaseline(Drawer::TextBaseline baseline) {
    Drawer::setTextBaseline(baseline);
    nvgTextAlign(ctx, toNVGAlign(baseline) | toNVGAlign(textAlign));
}

void NvgDrawer::registerFont(const char *name, const char *data, int dataSize) {
    nvgCreateFontMem(ctx, name, (unsigned char *) data, dataSize, 0);
}

void NvgDrawer::arc(float x, float y, float r, float sAngle, float eAngle) {
    nvgArc(ctx, x, y, r, sAngle, eAngle, NVG_CW);
}
