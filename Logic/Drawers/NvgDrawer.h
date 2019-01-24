//
// Created by Semyon Tikhonenko on 6/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_NVGOPENGLDRAWER_H
#define VOCALTRAINER_NVGOPENGLDRAWER_H

#include "Drawer.h"
#include <nanovg/nanovg.h>

class NvgDrawer : public Drawer {
#ifndef NDEBUG
    std::string fontFamily;
#endif
    CppUtils::Color fillColor;
protected:
    NVGcontext* ctx = nullptr;
    void setupBase();

    void doTranslate(float x, float y) override;
    void onImageDelete(Image *image) override;

    Color getFillColor() const override;

public:
    void beginFrame(float width, float height, float devicePixelRatio) override;
    void endFrame() override;
    void moveTo(float x, float y) override;
    void lineTo(float x, float y) override;
    void arcTo(float x1, float y1, float x2, float y2, float radius) override;

    void arc(float x, float y, float r, float sAngle, float eAngle) override;

    void setStrokeColor(const Color& color) override;
    void setFillColor(const Color& color) override;
    void setStrokeWidth(float strokeWidth) override;
    void stroke() override;
    void fill() override;
    void lineJoin(LineJoin type) override;

    void bezierCurveTo(float c1x, float c1y, float c2x, float c2y, float x, float y) override;
    void quadraticCurveTo(float cpx, float cpy, float x, float y) override;
    void beginPath() override;
    void closePath() override;
    void rotate(float angle) override;
    void scale(float x, float y) override;

    void roundedRect(float x, float y, float w, float h, float r) override;

    void drawTextUsingFonts(const std::string &text, float x, float y) override;

    void setTextFontFamily(const char *fontFamily) override;
    void setTextFontSize(float fontSize) override;
    void setTextWeight(int weight) override;
    void setTextAlign(TextAlign align) override;
    void setTextBaseline(TextBaseline baseline) override;
    void registerFont(const char *name, const char *data, int dataSize);

    void fillWithImage(Image *image, float textureX1, float textureY1, float textureX2, float textureY2) override;
    void drawImage(float x, float y, float w, float h, Image *image) override;
    Image *createImage(const void *data, int w, int h) override;

    void drawShadow(float x, float y, float w, float h, float radius, float blurFactor, const Color &color) override;

};

#endif //VOCALTRAINER_NVGOPENGLDRAWER_H
