//
// Created by Semyon Tikhonenko on 6/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_DRAWER_H
#define VOCALTRAINER_DRAWER_H

#include <array>

class Drawer {
public:
    typedef std::array<float, 4> Color;

    virtual void clear() = 0;
    virtual void beginFrame(float width, float height, float devicePixelRatio) = 0;
    virtual void endFrame() = 0;
    virtual void moveTo(float x, float y) = 0;
    virtual void lineTo(float x, float y) = 0;
    virtual void setStrokeColor(const Color& color) = 0;
    virtual void setStrokeWidth(float strokeWidth) = 0;
    virtual void stroke() = 0;
    virtual void beginPath() = 0;

    virtual ~Drawer() = default;
};


#endif //VOCALTRAINER_DRAWER_H
