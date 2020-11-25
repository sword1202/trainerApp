//
// Created by Semyon Tikhonenko on 11/24/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_MOUSECLICKCHECKER_H
#define VOCALTRAINER_MOUSECLICKCHECKER_H

#include "MouseEventsReceiver.h"
#include "Point.h"

class MouseClickChecker {
    MouseEventsReceiver* receiver;
    CppUtils::PointF clickMousePosition;
public:
    static inline CppUtils::PointF NO_CLICK = CppUtils::PointF(-1, -1);

    MouseClickChecker(MouseEventsReceiver *receiver);
    CppUtils::PointF getClickPosition();
};


#endif //VOCALTRAINER_MOUSECLICKCHECKER_H
