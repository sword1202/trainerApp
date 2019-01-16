//
// Created by Semyon Tikhonenko on 2019-01-15.
//

#include "QtWidgetMouseEventsReceiver.h"
#include <QEvent>
#include <QMouseEvent>

using namespace CppUtils;

QtWidgetMouseEventsReceiver::QtWidgetMouseEventsReceiver(QWidget *widget) : QObject(widget), widget(widget) {
    assert(widget);
    widget->installEventFilter(this);
}

bool QtWidgetMouseEventsReceiver::eventFilter(QObject *watched, QEvent *event) {
    if (auto* mouseEvent = dynamic_cast<QMouseEvent*>(event)) {
        const QPointF& position = mouseEvent->localPos();
        bool leftMouseDown = mouseEvent->buttons().testFlag(Qt::LeftButton);
        bool rightMouseDown = mouseEvent->buttons().testFlag(Qt::RightButton);
        PointF positionF(float(position.x()), float(position.y()));
        onMouseEvent(positionF, leftMouseDown, rightMouseDown);
    }

    return false;
}
