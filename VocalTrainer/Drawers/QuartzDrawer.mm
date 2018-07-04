//
// Created by Semyon Tikhonenko on 7/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "QuartzDrawer.h"

using namespace std;

void QuartzDrawer::translate(float x, float y) {
    CGContextTranslateCTM(ctx, x, height - y);
    translateX += x;
    translateY += y;
}

float QuartzDrawer::getTranslateX() {
    return 0;
}

float QuartzDrawer::getTranslateY() {
    return 0;
}

void QuartzDrawer::translateTo(float x, float y) {
    CGContextTranslateCTM(ctx, -translateX + x, height - (-translateY + y));
    translateX = x;
    translateY = y;
}

void QuartzDrawer::beginFrame(float width, float height, float devicePixelRatio) {
    this->width = width;
    this->height = height;
    translateX = 0;
    translateY = 0;
}

void QuartzDrawer::endFrame() {

}

void QuartzDrawer::moveTo(float x, float y) {
    CGContextMoveToPoint(ctx, x, height - y);
}

void QuartzDrawer::lineTo(float x, float y) {
    CGContextAddLineToPoint(ctx, x, height - y);
}

void QuartzDrawer::arcTo(float x1, float y1, float x2, float y2, float radius) {
    CGContextAddArcToPoint(ctx, x1, height - y1, x2, height - y2, radius);
}

void QuartzDrawer::setStrokeColor(const Drawer::Color &color) {
    CGContextSetRGBStrokeColor(ctx, color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, color[3] / 255.0f);
}

void QuartzDrawer::setFillColor(const Drawer::Color &color) {
    CGContextSetRGBFillColor(ctx, color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, color[3] / 255.0f);
}

void QuartzDrawer::setStrokeWidth(float strokeWidth) {
    CGContextSetLineWidth(ctx, strokeWidth);
}

void QuartzDrawer::stroke() {
    CGContextStrokePath(ctx);
}

void QuartzDrawer::fill() {
    CGContextFillPath(ctx);
}

void QuartzDrawer::beginPath() {
    CGContextBeginPath(ctx);
}

void QuartzDrawer::closePath() {
    CGContextClosePath(ctx);
}

void QuartzDrawer::bezierCurveTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {
    CGContextAddCurveToPoint(ctx, c1x, height - c1y, c2x, height - c2y, x, height - y);
}

void QuartzDrawer::lineJoin(Drawer::LineJoin type) {

}

void QuartzDrawer::rotate(float angle) {
    CGContextRotateCTM(ctx, angle);
}

void QuartzDrawer::scale(float x, float y) {
    CGContextScaleCTM(ctx, x, y);
}

void QuartzDrawer::rect(float x, float y, float w, float h) {
    CGContextAddRect(ctx, CGRectMake(x, height - y, w, -h));
}

void QuartzDrawer::fillRect(float x, float y, float w, float h) {
    CGContextFillRect(ctx, CGRectMake(x, height - y, w, -h));
}

QuartzDrawer::QuartzDrawer() {
    ctx = static_cast<CGContextRef>([[NSGraphicsContext currentContext] graphicsPort]);
}
