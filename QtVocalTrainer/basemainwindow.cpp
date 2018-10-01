#include "basemainwindow.h"
#include <QQmlContext>
#include <QGuiApplication>
#include <QScreen>

constexpr int MINIMUM_WINDOW_WIDTH = 700;
constexpr double MINIMUM_WINDOW_HEIGHT_RATIO = 0.6;

BaseMainWindow::BaseMainWindow(QColor windowBorderColor) {
#ifdef __APPLE__
    doMacOsPlatformStaff(windowBorderColor);
#endif

    cpp = new QmlCppBridge(this);

    QSize availableSize = QGuiApplication::primaryScreen()->availableSize();
    if (availableSize.width() > MINIMUM_WINDOW_WIDTH)
        setMinimumSize(MINIMUM_WINDOW_WIDTH, static_cast<int>(availableSize.height() * MINIMUM_WINDOW_HEIGHT_RATIO));
    else
        setMinimumSize(availableSize.width(), static_cast<int>(availableSize.height() * MINIMUM_WINDOW_HEIGHT_RATIO));
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
