//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_BASEMOUSEEVENTSRECEIVER_H
#define VOCALTRAINER_BASEMOUSEEVENTSRECEIVER_H

#include "MouseEventsReceiver.h"

class BaseMouseEventsReceiver : public MouseEventsReceiver {
    CppUtils::PointF currentMousePosition = CppUtils::PointF(0, 0);
    bool rightMouseDown = false;
    bool leftMouseDown = false;
public:
    bool isLeftMouseDown() override;
    bool isRightMouseDown() override;
    CppUtils::PointF getMousePosition() override;

    void setCurrentMousePosition(const CppUtils::PointF &currentMousePosition);
    void setRightMouseDown(bool rightMouseDown);
    void setLeftMouseDown(bool leftMouseDown);
};


#endif //VOCALTRAINER_BASEMOUSEEVENTSRECEIVER_H
