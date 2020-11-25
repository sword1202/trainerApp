//
// Created by Semyon Tikhonenko on 11/24/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#include "MouseClickChecker.h"

using namespace CppUtils;

MouseClickChecker::MouseClickChecker(MouseEventsReceiver *receiver) : receiver(receiver) {
    clickMousePosition = NO_CLICK;
}

CppUtils::PointF MouseClickChecker::getClickPosition() {
    if (receiver->isLeftMouseDown()) {
        if (clickMousePosition == NO_CLICK) {
            clickMousePosition = receiver->getMousePosition();
        } else if (!receiver->getMousePosition().compareUsingEpsilon(clickMousePosition, 0.1)) {
            // Click failed, invalidate point
            clickMousePosition = NO_CLICK;
        }
    } else {
        if (clickMousePosition != NO_CLICK) {
            auto result = clickMousePosition;
            clickMousePosition = NO_CLICK;
            return result;
        }
    }

    return NO_CLICK;
}
