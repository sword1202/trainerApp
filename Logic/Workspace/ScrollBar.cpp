//
// Created by Semyon Tikhonenko on 2019-01-15.
//

#include "ScrollBar.h"

using namespace CppUtils;

constexpr float SCROLLBAR_WEIGHT = 11.f;
constexpr float SCROLLBAR_PADDING = 2.f;

static const Color backgroundColor = {0xE8, 0xE7, 0xF0, 0xFF};
static const Color stripeColor = {0x61, 0x5F, 0x97, 0xFF};

ScrollBar::ScrollBar(Drawer *drawer, MouseEventsReceiver *mouseEventsReceiver, ScrollBar::Orientation orientation)
        : drawer(drawer), mouseEventsReceiver(mouseEventsReceiver), orientation(orientation) {

}

void ScrollBar::draw(float x, float y, float length) {
    bool isVertical = orientation == VERTICAL;

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
    scrollerStripeRect.translate(-drawer->getTranslateX(), -drawer->getTranslateY());

    if (mouseEventsReceiver->isLeftMouseDown()) {
        if (leftMouseWasDownOnScroller) {
            PointF delta = mouseEventsReceiver->getMousePositionDelta();
            float lengthDelta = isVertical ? delta.y : delta.x;
            float positionDelta = lengthDelta / maxPositionTranslation;
            position += positionDelta;
            if (position < 0) {
                position = 0;
            } else if(position > 1) {
                position = 1;
            }

        } else {
            leftMouseWasDownOnScroller = scrollerStripeRect.containsPoint(mouseEventsReceiver->getMousePosition());
        }
    } else {
        leftMouseWasDownOnScroller = false;
    }

    // translate back to local coordinates
    scrollerStripeRect.translate(drawer->getTranslateX(), drawer->getTranslateY());
    applyPositionTranslation();

    drawer->roundedRect(scrollerStripeRect);
    drawer->fill();
}
