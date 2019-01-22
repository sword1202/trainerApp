//
// Created by Semyon Tikhonenko on 2019-01-15.
//

#ifndef MVXGENERATOR_HORIZONTALSCROLLBAR_H
#define MVXGENERATOR_HORIZONTALSCROLLBAR_H

#include "Drawer.h"
#include "MouseEventsReceiver.h"

class ScrollBar {
public:
    enum Orientation {
        VERTICAL,
        HORIZONTAL
    };
private:
    std::atomic<float> position;
    std::atomic<float> pageSize;
    Drawer* drawer;
    MouseEventsReceiver* mouseEventsReceiver;
    CppUtils::PointF lastMousePosition;
    Orientation orientation;
    bool leftMouseWasDownOnScroller = false;
    bool positionWasChangedFromUser = false;
public:
    static constexpr float SCROLLBAR_WEIGHT = 11.f;

    ScrollBar(Drawer *drawer,
            MouseEventsReceiver *mouseEventsReceiver,
            Orientation orientation);
    void draw(float x, float y, float length);

    float getPageSize() const;
    void setPageSize(float pageSize);

    float getPosition() const;
    void setPosition(float position);

    bool isPositionWasChangedFromUser() const;
};


#endif //MVXGENERATOR_HORIZONTALSCROLLBAR_H
