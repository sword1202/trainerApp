//
// Created by Semyon Tikhonenko on 2018-12-16.
//

#include "SingingResultDialog.h"
#include "Utils/BaseQmlWidget.h"
#include <QQmlContext>

constexpr int DIALOG_WIDTH = 385;
constexpr int DIALOG_HEIGHT = 290;

SingingResultDialog::SingingResultDialog(QWidget* parent) : QDialog(parent) {
    setAttribute(Qt::WA_DeleteOnClose, true);

    auto *qmlWidget = new BaseQmlWidget(this);
    QQmlContext *context = qmlWidget->rootContext();
    context->setContextProperty("self", this);
    qmlWidget->setResizeMode(QQuickWidget::SizeViewToRootObject);
    setFixedSize(DIALOG_WIDTH, DIALOG_HEIGHT);
    qmlWidget->setSource(QUrl("qrc:/qml/Project/SingingResultDialog.qml"));
}