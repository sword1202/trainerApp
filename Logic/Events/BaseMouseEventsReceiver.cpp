//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#include "BaseMouseEventsReceiver.h"

bool BaseMouseEventsReceiver::isLeftMouseDown() {
    return false;
}

bool BaseMouseEventsReceiver::isRightMouseDown() {
    return false;
}

CppUtils::PointF BaseMouseEventsReceiver::getMousePosition() {
    return CppUtils::PointF();
}

void BaseMouseEventsReceiver::setCurrentMousePosition(const CppUtils::PointF &currentMousePosition) {
    this->currentMousePosition = currentMousePosition;
}

void BaseMouseEventsReceiver::setRightMouseDown(bool rightMouseDown) {
    this->rightMouseDown = rightMouseDown;
}

void BaseMouseEventsReceiver::setLeftMouseDown(bool leftMouseDown) {
    this->leftMouseDown = leftMouseDown;
}
