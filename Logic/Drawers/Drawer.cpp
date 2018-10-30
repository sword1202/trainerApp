#define _USE_MATH_DEFINES
#include "Drawer.h"
#include "Sets.h"
#include "Core.h"
#include <iostream>

using namespace CppUtils;
using namespace std;

void Drawer::roundedRect(float x, float y, float w, float h, float r) {
    assert(r >= 0);
    assert(w >= 0);
    assert(h >= 0);

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
    if (!image) {
        return;
    }

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
    translateX = 0;
    translateY = 0;
}

void Drawer::rect(float x, float y, float w, float h) {
    beginPath();
    moveTo(x, y);
    lineTo(x + w, y);
    lineTo(x + w, y + h);
    lineTo(x, y + h);
    closePath();
}

void Drawer::fillRect(float x, float y, float w, float h) {
    rect(x, y, w, h);
    fill();
}

void Drawer::translate(float x, float y) {
    doTranslate(x, y);
    translateX += x;
    translateY += y;
}

float Drawer::getTranslateX() {
    return translateX;
}

float Drawer::getTranslateY() {
    return translateY;
}

void Drawer::translateTo(float x, float y) {
    doTranslate(-translateX + x, -translateY + y);
    translateX = x;
    translateY = y;
}

void Drawer::clear() {
    setFillColor(Color::white());
    fillRect(0, 0, width, height);
}

void Drawer::circle(float x, float y, float r) {
    beginPath();
    arc(x, y, r, 0, 2 * M_PI);
}

void Drawer::deleteImage(Drawer::Image *&image) {
    onImageDelete(image);
    images.erase(image);
    delete image;
    image = nullptr;
}

Drawer::Image * Drawer::registerImage(Drawer::Image *image) {
    images.insert(image);
    return image;
}

void Drawer::onImageDelete(Drawer::Image *image) {

}

Drawer::~Drawer() {
    for (Image* image : images) {
        onImageDelete(image);
        delete image;
    }

    CheckedDelete(textImagesFactory);
}

bool Drawer::imageRegistered(Drawer::Image *image) const {
    return (bool)images.count(image);
}

void Drawer::setTextAlign(Drawer::TextAlign align) {
    textAlign = align;
}

Drawer::TextDrawStrategy Drawer::getTextDrawStrategy() const {
    return textDrawStrategy;
}

void Drawer::setTextDrawStrategy(Drawer::TextDrawStrategy textDrawStrategy) {
    assert((textDrawStrategy != DRAW_USING_PRE_BUILD_IMAGES || textImagesFactory) &&
    "call setTextImagesFactory before setting DRAW_USING_PRE_BUILD_IMAGES");
    this->textDrawStrategy = textDrawStrategy;
}

void Drawer::setTextFontFamily(const char *fontFamily) {

}

void Drawer::setTextFontSize(float fontSize) {
    assert(fontSize > 0);
    this->fontSize = fontSize;
}

void Drawer::setTextImagesFactory(DrawerTextImagesFactory *textImagesFactory) {
    assert(textImagesFactory);
    CheckedDelete(this->textImagesFactory);
    this->textImagesFactory = textImagesFactory;
}

void Drawer::fillText(const std::string &text, float x, float y) {
    if (textDrawStrategy == DRAW_USING_FONTS) {
        drawTextUsingFonts(text, x, y);
    } else if(textDrawStrategy == DRAW_USING_PRE_BUILD_IMAGES) {
        drawTextUsingImages(text, x, y);
    }
}

static constexpr float LETTER_SPACING_FACTOR = 0.5f;

void Drawer::drawTextUsingImages(const std::string &text, float x, float y) {
    tempTextImages.clear();
    float height = 0;
    float width = 0;
    for (char ch : text) {
        Image* image = textImagesFactory->findImage(ch, (int)round(fontSize));
        assert(image);
        if (height < image->height()) {
            height = image->height();
        }
        width += image->width();
        tempTextImages.push_back(image);
    }
    height /= devicePixelRatio;
    width /= devicePixelRatio;

    width += (tempTextImages.size() - 1) * LETTER_SPACING_FACTOR;

    if (textAlign == CENTER) {
        x -= width / 2;
    } else if(textAlign == RIGHT) {
        x -= width;
    }
    if (textBaseline == MIDDLE) {
        y -= height / 2;
    } else if(textBaseline == BOTTOM) {
        y -= height;
    }

    for (Image* image : tempTextImages) {
        drawImage(x, y, image);
        x += image->width() / devicePixelRatio + LETTER_SPACING_FACTOR;
    }
}

Drawer::Drawer() {
    tempTextImages.reserve(10);
}

void Drawer::drawImage(float x, float y, Drawer::Image *image) {
    drawImage(x, y, image->width() / devicePixelRatio, image->height() / devicePixelRatio, image);
}

void Drawer::setTextBaseline(Drawer::TextBaseline baseline) {
    textBaseline = baseline;
}

void Drawer::test() {

}

void DrawerTextImagesFactory::addImage(const DrawerTextImagesFactoryCharacterData &data) {
    set.insert(data);
}

Drawer::Image *DrawerTextImagesFactory::findImage(char character, int fontSize) {
    DrawerTextImagesFactoryCharacterData stub;
    stub.character = character;
    stub.fontSize = fontSize;
    return Sets::FindOrDefault(set, stub).image;
}

bool DrawerTextImagesFactoryCharacterData::operator==(const DrawerTextImagesFactoryCharacterData &rhs) const {
    return character == rhs.character &&
            fontSize == rhs.fontSize;
}

bool DrawerTextImagesFactoryCharacterData::operator!=(const DrawerTextImagesFactoryCharacterData &rhs) const {
    return !(rhs == *this);
}
