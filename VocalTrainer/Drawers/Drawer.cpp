#include "Drawer.h"

using namespace CppUtils;

void Drawer::roundedRect(float x, float y, float w, float h, float r) {
    assert(r >= 0);

    if (w < 2 * r) {
        r = w / 2;
    }
    if (h < 2 * r) {
        r = h / 2;
    }

    beginPath();
    moveTo(x+r, y);
    arcTo(x+w, y,   x+w, y+h, r);
    arcTo(x+w, y+h, x,   y+h, r);
    arcTo(x,   y+h, x,   y,   r);
    arcTo(x,   y,   x+w, y,   r);
    closePath();
}

void Drawer::roundedRect(const RoundedRectF &rect) {
    roundedRect(rect.A.x, rect.A.x, rect.width, rect.height, rect.getRadius());
}

void Drawer::lineTo(const CppUtils::PointF &point) {
    lineTo(point.x, point.y);
}

void Drawer::moveTo(const CppUtils::PointF &point) {
    moveTo(point.x, point.y);
}

void Drawer::roundedRectDifferentCorners(float x, float y, float w,
        float h, float radiusLeftTop,
        float radiusRightTop, float radiusBottomRight, float radiusBottomLeft) {
    beginPath();
    moveTo(x+radiusRightTop, y);
    arcTo(x+w, y,   x+w, y+h, radiusRightTop);
    arcTo(x+w, y+h, x,   y+h, radiusBottomRight);
    arcTo(x,   y+h, x,   y,   radiusBottomLeft);
    arcTo(x,   y,   x+w, y,   radiusLeftTop);
    closePath();
}

void Drawer::setTextFont(const std::string &fontFamily, float fontSize) {
    this->fontFamily = fontFamily;
    this->fontSize = fontSize;
}

void Drawer::setTextAlign(Drawer::TextAlign align) {
    textAlign = align;
}

void Drawer::setTextBaseline(Drawer::TextBaseline baseline) {
    textBaseline = baseline;
}

void Drawer::drawLine(float x1, float y1, float x2, float y2) {
    beginPath();
    moveTo(x1, y1);
    lineTo(x2, y2);
    stroke();
}

void Drawer::drawVerticalLine(float x, float y, float height) {
    drawLine(x, y, x, y + height);
}

void Drawer::setStrokeColor(int color) {
    setStrokeColor(DrawerColor::fromHex(color));
}

void Drawer::setFillColor(int color) {
    setFillColor(DrawerColor::fromHex(color));
}

void Drawer::fillWithImage(Drawer::Image *image) {
    fillWithImage(image, 0, 0, image->width() / devicePixelRatio, image->height() / devicePixelRatio);
}

float Drawer::getWidth() const {
    return width;
}

float Drawer::getHeight() const {
    return height;
}

float Drawer::getDevicePixelRatio() const {
    return devicePixelRatio;
}

void Drawer::beginFrame(float width, float height, float devicePixelRatio) {
    this->width = width;
    this->height = height;
    this->devicePixelRatio = devicePixelRatio;
}
