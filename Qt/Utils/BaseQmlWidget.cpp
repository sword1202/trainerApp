//
// Created by Semyon Tikhonenko on 2018-12-15.
//

#include "BaseQmlWidget.h"
#include <QQmlContext>
#include "QtBridge/QtCppBridge.h"

BaseQmlWidget::BaseQmlWidget(QWidget *parent) : QQuickWidget(parent) {
    rootContext()->setContextProperty("cpp", new QtCppBridge(this));
    rootContext()->setContextProperty("self", this);
}
