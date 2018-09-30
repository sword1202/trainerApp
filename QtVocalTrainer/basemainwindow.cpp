#include "basemainwindow.h"
#include <QQmlContext>

BaseMainWindow::BaseMainWindow(QColor windowBorderColor) {
#ifdef __APPLE__
    doMacOsPlatformStaff(windowBorderColor);
#endif

    cpp = new QmlCppBridge(this);
}

QQuickWidget *BaseMainWindow::createQQuickWidget(const QString& qmlFile) {
    return createQQuickWidget(qmlFile, this);
}

QQuickWidget *BaseMainWindow::createQQuickWidget(const QString &qmlFile, QWidget *parent) {
    QQuickWidget* qmlWidget = new QQuickWidget(parent);
    qmlWidget->rootContext()->setContextProperty("cpp", cpp);
    qmlWidget->setSource(QUrl(qmlFile));
    return qmlWidget;
}
