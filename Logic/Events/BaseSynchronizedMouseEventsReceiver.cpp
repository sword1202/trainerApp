//
// Created by Semyon Tikhonenko on 2019-01-15.
//

#include "BaseSynchronizedMouseEventsReceiver.h"

#define LOCK std::lock_guard<std::mutex> _(mousePositionMutex)

using namespace CppUtils;

BaseSynchronizedMouseEventsReceiver::BaseSynchronizedMouseEventsReceiver() {
    leftMouseDown = false;
    rightMouseDown = false;
}

bool BaseSynchronizedMouseEventsReceiver::isLeftMouseDown() {
    LOCK;
    return leftMouseDown;
}

bool BaseSynchronizedMouseEventsReceiver::isRightMouseDown() {
    LOCK;
    return rightMouseDown;
}

PointF BaseSynchronizedMouseEventsReceiver::getMousePosition() {
    LOCK;
    return currentMousePosition;
}

PointF BaseSynchronizedMouseEventsReceiver::getPreviousMousePosition() {
    LOCK;
    return previousMousePosition;
}

PointF BaseSynchronizedMouseEventsReceiver::getMousePositionDelta() {
    LOCK;
    if (previousMousePosition.x < 0 || currentMousePosition.x < 0) {
        return {-1, -1};
    }

    return currentMousePosition - previousMousePosition;
}

void BaseSynchronizedMouseEventsReceiver::onMouseEvent(const CppUtils::PointF &position,
        bool leftButtonDown, bool rightButtonDown) {
    LOCK;
    previousMousePosition = currentMousePosition;
    currentMousePosition = position;
    leftMouseDown = leftButtonDown;
    rightMouseDown = rightButtonDown;
}
