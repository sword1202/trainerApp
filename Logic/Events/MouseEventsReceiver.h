//
// Created by Semyon Tikhonenko on 2019-01-15.
//

#ifndef MVXGENERATOR_MOUSEEVENTSRECEIVER_H
#define MVXGENERATOR_MOUSEEVENTSRECEIVER_H

#include "Point.h"

class MouseEventsReceiver {
public:
    virtual bool isLeftMouseDown() = 0;
    virtual bool isRightMouseDown() = 0;
    virtual CppUtils::PointF getMousePosition() = 0;
    virtual CppUtils::PointF getPreviousMousePosition() = 0;
    virtual CppUtils::PointF getMousePositionDelta() = 0;
    virtual ~MouseEventsReceiver() = default;
};


#endif //MVXGENERATOR_MOUSEEVENTSRECEIVER_H
