//
// Created by Semyon Tikhonenko on 2019-01-15.
//

#ifndef MVXGENERATOR_QTWIDGETMOUSEEVENTSRECEIVER_H
#define MVXGENERATOR_QTWIDGETMOUSEEVENTSRECEIVER_H

#include "BaseSynchronizedMouseEventsReceiver.h"
#include <QWidget>

class QtWidgetMouseEventsReceiver : public BaseSynchronizedMouseEventsReceiver, public QObject {
    QWidget* widget;
public:
    explicit QtWidgetMouseEventsReceiver(QWidget *widget);
    bool eventFilter(QObject *watched, QEvent *event) override;
};


#endif //MVXGENERATOR_QTWIDGETMOUSEEVENTSRECEIVER_H
