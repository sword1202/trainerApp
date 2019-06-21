//
// Created by Semyon Tykhonenko on 2019-06-08.
//

#include "BaseQmlDialog.h"
#include <QQuickWidget>
#include <QQmlContext>

BaseQmlDialog::BaseQmlDialog(QWidget *parent, QtCppBridge *cpp, const char* qmlPath) : QDialog(parent) {
    auto* quickWidget = new QQuickWidget(this);
    context = quickWidget->rootContext();
    context->setContextProperty("self", this);
    context->setContextProperty("cpp", cpp);

    quickWidget->setSource(QUrl(qmlPath));
    rootQml = quickWidget->rootObject();

    setAttribute(Qt::WA_DeleteOnClose, true);
    setModal(true);
}

QQuickItem *BaseQmlDialog::getRootQml() {
    return rootQml;
}

QQmlContext *BaseQmlDialog::getContext() {
    return context;
}
