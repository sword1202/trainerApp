//
// Created by Semyon Tikhonenko on 2019-01-15.
//

#ifndef MVXGENERATOR_BASESYNCHRONIZEDMOUSEEVENTSRECEIVER_H
#define MVXGENERATOR_BASESYNCHRONIZEDMOUSEEVENTSRECEIVER_H

#include "MouseEventsReceiver.h"
#include <array>
#include <mutex>

class BaseSynchronizedMouseEventsReceiver : public MouseEventsReceiver {
    CppUtils::PointF currentMousePosition;
    std::mutex mousePositionMutex;
    bool rightMouseDown;
    bool leftMouseDown;

protected:
    void onMouseEvent(const CppUtils::PointF &position, bool leftButtonDown, bool rightButtonDown);
public:
    BaseSynchronizedMouseEventsReceiver();

    bool isLeftMouseDown() override;
    bool isRightMouseDown() override;

    CppUtils::PointF getMousePosition() override;
};


#endif //MVXGENERATOR_BASESYNCHRONIZEDMOUSEEVENTSRECEIVER_H
