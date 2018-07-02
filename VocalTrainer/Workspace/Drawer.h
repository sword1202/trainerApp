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

class Drawer {
public:
    enum LineJoin {
        BEVEL, ROUND, MITER
    };

    typedef std::array<unsigned char, 4> Color;

    virtual void translate(float x, float y) = 0;
    virtual float getTranslateX() = 0;
    virtual float getTranslateY() = 0;
    virtual void translateTo(float x, float y) = 0;

    virtual void clear() = 0;
    virtual void beginFrame(float width, float height, float devicePixelRatio) = 0;
    virtual void endFrame() = 0;
    virtual void moveTo(float x, float y) = 0;
    virtual void lineTo(float x, float y) = 0;
    virtual void arcTo(float x1, float y1, float x2, float y2, float radius) = 0;
    virtual void setStrokeColor(const Color& color) = 0;
    virtual void setFillColor(const Color& color) = 0;
    virtual void setStrokeWidth(float strokeWidth) = 0;
    virtual void stroke() = 0;
    virtual void fill() = 0;
    virtual void beginPath() = 0;
    virtual void closePath() = 0;
    virtual void bezierCurveTo(float c1x, float c1y, float c2x, float c2y, float x, float y) = 0;
    virtual void lineJoin(LineJoin type) = 0;
    virtual void rotate(float angle) = 0;
    virtual void scale(float x, float y) = 0;
    virtual void rect(float x, float y, float w, float h) = 0;
    virtual void fillRect(float x, float y, float w, float h) = 0;

    void roundedRect(float x, float y, float w, float h, float r);
    void roundedRect(const CppUtils::RoundedRectF& roundedRect);
    virtual void lineTo(const CppUtils::PointF& point);
    virtual void moveTo(const CppUtils::PointF& point);

    virtual ~Drawer() = default;
};


#endif //VOCALTRAINER_DRAWER_H
