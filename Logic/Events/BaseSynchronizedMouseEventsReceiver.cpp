//
// Created by Semyon Tikhonenko on 2019-01-15.
//

#include "BaseSynchronizedMouseEventsReceiver.h"

//#define LOCK std::lock_guard<std::mutex> _(mousePositionMutex)
#define LOCK

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

void BaseSynchronizedMouseEventsReceiver::onMouseEvent(const CppUtils::PointF &position,
        bool leftButtonDown, bool rightButtonDown) {
    LOCK;
    currentMousePosition = position;
    leftMouseDown = leftButtonDown;
    rightMouseDown = rightButtonDown;
}
