//
// Created by Semyon Tikhonenko on 6/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_DRAWER_H
#define VOCALTRAINER_DRAWER_H

#include <array>
#include <vector>
#include "RoundedRect.h"
#include "Point.h"
#include "DrawerColor.h"
#include <string>

class Drawer {
public:
    class Image {
    public:
        virtual int width() = 0;
        virtual int height() = 0;
        virtual ~Image() = default;
    };

    enum LineJoin {
        BEVEL, ROUND, MITER
    };

    enum TextAlign {
        CENTER, LEFT, RIGHT
    };

    enum TextBaseline {
        TOP, BOTTOM, MIDDLE
    };

    typedef DrawerColor Color;

    virtual void clear();

    virtual void translate(float x, float y);
    virtual float getTranslateX();
    virtual float getTranslateY();
    virtual void translateTo(float x, float y);

    virtual void beginFrame(float width, float height, float devicePixelRatio);
    virtual void endFrame() = 0;
    virtual void moveTo(float x, float y) = 0;
    virtual void lineTo(float x, float y) = 0;
    virtual void arcTo(float x1, float y1, float x2, float y2, float radius) = 0;
    virtual void arc(float x, float y, float r, float sAngle, float eAngle) = 0;
    virtual void circle(float x, float y, float r);
    virtual void setStrokeColor(int color);
    virtual void setFillColor(int color);
    virtual void setStrokeColor(const Color& color) = 0;
    virtual void setFillColor(const Color& color) = 0;
    virtual void setStrokeWidth(float strokeWidth) = 0;
    virtual void stroke() = 0;
    virtual void fill() = 0;
    virtual void fillWithImage(Image* image, float textureX1, float textureY1, float textureX2, float textureY2) = 0;
    virtual void fillWithImage(Image* image);
    virtual void drawImage(float x, float y, float w, float h, Image *image) = 0;
    virtual void beginPath() = 0;
    virtual void closePath() = 0;
    virtual void bezierCurveTo(float c1x, float c1y, float c2x, float c2y, float x, float y) = 0;
    virtual void quadraticCurveTo(float cpx, float cpy, float x, float y) = 0;
    virtual void lineJoin(LineJoin type) = 0;
    virtual void rotate(float angle) = 0;
    virtual void scale(float x, float y) = 0;
    virtual void rect(float x, float y, float w, float h);
    virtual void fillRect(float x, float y, float w, float h);
    virtual void drawLine(float x1, float y1, float x2, float y2);
    virtual void drawVerticalLine(float x, float y, float height);

    virtual void roundedRect(float x, float y, float w, float h, float r);
    virtual void roundedRectDifferentCorners(float x, float y, float w,
            float h, float radiusLeftTop,
            float radiusRightTop, float radiusBottomRight, float radiusBottomLeft);
    void roundedRect(const CppUtils::RoundedRectF& roundedRect);
    virtual void lineTo(const CppUtils::PointF& point);
    virtual void moveTo(const CppUtils::PointF& point);

    virtual void setTextFontFamily(const char* fontFamily) = 0;
    virtual void setTextFontSize(float fontSize) = 0;
    virtual void setTextAlign(TextAlign align) = 0;
    virtual void setTextWeight(int weight) = 0;
    virtual void setTextBaseline(TextBaseline baseline) = 0;
    virtual void fillText(const std::string &text, float x, float y) = 0;

    virtual Image* createImage(const void* data, int w, int h) = 0;
    virtual void deleteImage(Image*& image);

    virtual ~Drawer();

protected:

    float getWidth() const;
    float getHeight() const;
    float getDevicePixelRatio() const;

    virtual void doTranslate(float x, float y) = 0;
    virtual Image* registerImage(Image* image);
    virtual bool imageRegistered(Image* image) const;
    virtual void onImageDelete(Image* image);
private:
    float width;
    float height;
    float devicePixelRatio;

    float translateX = 0;
    float translateY = 0;

    std::unordered_set<Image*> images;
};


#endif //VOCALTRAINER_DRAWER_H
