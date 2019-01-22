//
// Created by Semyon Tikhonenko on 2019-01-15.
//

#include "ScrollBar.h"

using namespace CppUtils;

constexpr float SCROLLBAR_PADDING = 2.f;

static const Color backgroundColor = {0xE8, 0xE7, 0xF0, 0xFF};
static const Color stripeColor = {0x61, 0x5F, 0x97, 0xFF};

ScrollBar::ScrollBar(Drawer *drawer, MouseEventsReceiver *mouseEventsReceiver, ScrollBar::Orientation orientation)
        : drawer(drawer), mouseEventsReceiver(mouseEventsReceiver), orientation(orientation) {
    pageSize = 0;
    position = 0;
}

void ScrollBar::draw(float x, float y, float length) {
    assert(pageSize > 0 && "Call setPageSize before draw");
    bool isVertical = orientation == VERTICAL;
    float position = this->position;
    float pageSize = this->pageSize;

    drawer->setFillColor(backgroundColor);
    drawer->fillRect(x, y, isVertical ? SCROLLBAR_WEIGHT : length, isVertical ? length : SCROLLBAR_WEIGHT);

    drawer->setFillColor(stripeColor);
    float stripeWeight = SCROLLBAR_WEIGHT - SCROLLBAR_PADDING * 2;
    float stripeX = x + SCROLLBAR_PADDING;
    float stripeY = y + SCROLLBAR_PADDING;

    float stripeMaxLength = length - SCROLLBAR_PADDING * 2;
    float maxPositionTranslation = (stripeMaxLength * (1.0f - pageSize));
    float stripeLength = stripeMaxLength * pageSize;
    float stripeWidth = isVertical ? stripeWeight : stripeLength;
    float stripeHeight = isVertical ? stripeLength : stripeWeight;
    float rectRadius = stripeWeight / 2;

    RoundedRectF scrollerStripeRect(PointF(stripeX, stripeY), stripeWidth, stripeHeight, rectRadius);

    auto applyPositionTranslation = [&] {
        float translation = position * maxPositionTranslation;
        if (isVertical) {
            scrollerStripeRect.A.y = stripeY + translation;
        } else {
            scrollerStripeRect.A.x = stripeX + translation;
        }
    };
    applyPositionTranslation();

    // translate to global coordinates
    scrollerStripeRect.translate(drawer->getTranslateX(), drawer->getTranslateY());

    if (mouseEventsReceiver->isLeftMouseDown()) {
        if (leftMouseWasDownOnScroller) {
            auto currentMousePosition = mouseEventsReceiver->getMousePosition();
            PointF delta = currentMousePosition - lastMousePosition;
            lastMousePosition = currentMousePosition;
            float lengthDelta = isVertical ? delta.y : delta.x;
            float positionDelta = lengthDelta / maxPositionTranslation;
            position += positionDelta;
            if (position < 0) {
                position = 0;
            } else if(position > 1) {
                position = 1;
            }

        } else {
            lastMousePosition = mouseEventsReceiver->getMousePosition();
            leftMouseWasDownOnScroller = scrollerStripeRect.containsPoint(lastMousePosition);
        }
    } else {
        leftMouseWasDownOnScroller = false;
    }

    // translate back to local coordinates
    scrollerStripeRect.translate(drawer->getTranslateX(), drawer->getTranslateY());
    applyPositionTranslation();

    drawer->roundedRect(scrollerStripeRect);
    drawer->fill();
    this->position = position;
}

float ScrollBar::getPageSize() const {
    return pageSize;
}

void ScrollBar::setPageSize(float pageSize) {
    assert(pageSize > 0);
    this->pageSize = pageSize;
}

float ScrollBar::getPosition() const {
    return position;
}

void ScrollBar::setPosition(float position) {
    assert(position >= 0 && position <= 1);
    this->position = position;
}
