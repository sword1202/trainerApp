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

#include "config.h"

class CPP_UTILS_DLLHIDE NvgImage : public Drawer::Image {
    int w;
    int h;
public:
    NvgImage(int handle, int w, int h) : handle(handle), w(w), h(h) {
    }

    int handle = -1;

    int width() override {
        return w;
    }

    int height() override {
        return h;
    }
};

#ifdef __APPLE__

#include <nanovg/nanovg_mtl.h>

void MetalNvgDrawer::clear() {
    mnvgClearWithColor(ctx, nvgRGBA(255, 255, 255, 255));
}

MetalNvgDrawer::MetalNvgDrawer(void* layer) {
    ctx = nvgCreateMTL(layer, NVG_ANTIALIAS);
}

MetalNvgDrawer::~MetalNvgDrawer() {
    nvgDeleteMTL(ctx);
}

#endif


#ifdef _WIN32
#define NANOVG_GL3_IMPLEMENTATION
#include <GLEW/GL/glew.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#define NANOVG_GL2_IMPLEMENTATION
#endif

#include <nanovg/nanovg_gl.h>
#include <nanovg/fontstash.h>
#include <NotImplementedAssert.h>

OpenGLNvgDrawer::OpenGLNvgDrawer() {
#ifdef _WIN32
    GLint GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult) {
        const GLubyte *er = glewGetErrorString(GlewInitResult);
        //qDebug() << "ERROR: " << reinterpret_cast<const char *>(er);
    }
    ctx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);

#endif

#ifdef __APPLE__
    ctx = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#endif

    setupBase();
}

void OpenGLNvgDrawer::clear() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void NvgDrawer::setupBase() {
    setTextAlign(textAlign);
    setTextBaseline(textBaseline);
}

OpenGLNvgDrawer::~OpenGLNvgDrawer() {

#ifdef _WIN32
    nvgDeleteGL3(ctx);
#endif

#ifdef __APPLE__
    nvgDeleteGL2(ctx);
#endif
}

void NvgDrawer::beginFrame(float width, float height, float devicePixelRatio) {
    Drawer::beginFrame(width, height, devicePixelRatio);
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

void NvgDrawer::arcTo(float x1, float y1, float x2, float y2, float radius) {
    assert(radius >= 0);
    nvgArcTo(ctx, x1, y1, x2, y2, radius);
}

void NvgDrawer::roundedRect(float x, float y, float w, float h, float r) {
    Drawer::roundedRect(x, y, w, h, r);
}

void NvgDrawer::quadraticCurveTo(float cpx, float cpy, float x, float y) {
    nvgQuadTo(ctx, cpx, cpy, x, y);
}

void NvgDrawer::drawTextUsingFonts(const std::string &text, float x, float y) {
#ifndef NDEBUG
    if (nvgFindFont(ctx, fontFamily.data()) == FONS_INVALID){
        throw std::runtime_error("Font " + fontFamily + " was not registered, call drawer->registerFont before fillText call");
    }
#endif

    nvgText(ctx, x, y, text.data(), text.data() + text.size());
}

void NvgDrawer::setTextFontFamily(const char *fontFamily) {
    if (getTextDrawStrategy() == DRAW_USING_PRE_BUILD_IMAGES) {
        Drawer::setTextFontFamily(fontFamily);
        return;
    }

#ifndef NDEBUG
    this->fontFamily.assign(fontFamily);
#endif
    nvgFontFace(ctx, fontFamily);
}

void NvgDrawer::setTextFontSize(float fontSize) {
    if (getTextDrawStrategy() == DRAW_USING_PRE_BUILD_IMAGES) {
        Drawer::setTextFontSize(fontSize);
        return;
    }
    assert(fontSize > 0);
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

void NvgDrawer::fillWithImage(Drawer::Image *image, float textureX1, float textureY1, float textureX2, float textureY2) {
    assert(dynamic_cast<NvgImage*>(image));
    assert(imageRegistered(image));
    NVGpaint nvGpaint = nvgImagePattern(ctx, textureX1, textureY1, textureX2, textureY2, 0, 
            static_cast<NvgImage*>(image)->handle, 1);
    nvgFillPaint(ctx, nvGpaint);
    fill();
}

Drawer::Image *NvgDrawer::createImage(const void *data, int w, int h) {
    int handle = nvgCreateImageRGBA(ctx, w, h, 0, (unsigned char *) data);
    NvgImage* image = new NvgImage(handle, w, h);
    registerImage(image);
    return image;
}

void NvgDrawer::doTranslate(float x, float y) {
    nvgTranslate(ctx, x, y);
}

void NvgDrawer::setTextWeight(int weight) {
    // Not supported
}

void NvgDrawer::onImageDelete(Drawer::Image *image) {
    assert(dynamic_cast<NvgImage*>(image));
    nvgDeleteImage(ctx, static_cast<NvgImage*>(image)->handle);
}

void NvgDrawer::drawImage(float x, float y, float w, float h, Drawer::Image *image) {
    // nanovg can't draw image with x, y != 0, strange bug, use translate for a workground
    translate(x, y);
    rect(0, 0, w, h);
    fillWithImage(image, 0, 0, w, h);
    translate(-x, -y);
}

void NvgDrawer::test() {

}
