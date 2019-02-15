#include "BaseMainWindow.h"
#include <QQmlContext>
#include <QGuiApplication>
#include <QScreen>

constexpr int MINIMUM_WINDOW_WIDTH = 700;
constexpr double MINIMUM_WINDOW_HEIGHT_RATIO = 0.6;

BaseMainWindow::BaseMainWindow(QColor windowBorderColor) {
#ifdef __APPLE__
    setWindowBorderColor(windowBorderColor);
#endif

    cpp = new QtCppBridge(this);

    QSize availableSize = QGuiApplication::primaryScreen()->availableSize();
    if (availableSize.width() > MINIMUM_WINDOW_WIDTH)
        setMinimumSize(MINIMUM_WINDOW_WIDTH, static_cast<int>(availableSize.height() * MINIMUM_WINDOW_HEIGHT_RATIO));
    else
        setMinimumSize(availableSize.width(), static_cast<int>(availableSize.height() * MINIMUM_WINDOW_HEIGHT_RATIO));
}

QQuickWidget *BaseMainWindow::createQQuickWidget(const QString& qmlFile, const std::function<void(QQmlContext* context)>& initContext) {
    return createQQuickWidget(qmlFile, this, initContext);
}

QQuickWidget *BaseMainWindow::createQQuickWidget(const QString &qmlFile, QWidget *parent, const std::function<void(QQmlContext* context)>& initContext) {
    QQuickWidget* qmlWidget = new QQuickWidget(parent);
    QQmlContext *context = qmlWidget->rootContext();
    context->setContextProperty("cpp", cpp);
    context->setContextProperty("self", this);
    if (initContext) {
        initContext(context);
    }
    qmlWidget->setSource(QUrl(qmlFile));
    return qmlWidget;
}
