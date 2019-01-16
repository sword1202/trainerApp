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
    float position = 0;
    float pageSize = 0;
    Drawer* drawer;
    MouseEventsReceiver* mouseEventsReceiver;
    Orientation orientation;
    bool leftMouseWasDownOnScroller = false;
public:
    ScrollBar(Drawer *drawer,
            MouseEventsReceiver *mouseEventsReceiver,
            Orientation orientation);
    void draw(float x, float y, float length);
};


#endif //MVXGENERATOR_HORIZONTALSCROLLBAR_H
