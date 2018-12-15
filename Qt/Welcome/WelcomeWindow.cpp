#include "WelcomeWindow.h"

#include <QFontDatabase>
#include <QGraphicsEffect>
#include <QScreen>
#include <QLabel>
#include <QGuiApplication>

#include "App/AppSettings.h"

constexpr double WINDOW_SIZE_RATIO = 0.8;

WelcomeWindow::WelcomeWindow() :
    BaseMainWindow(QColor::fromRgb(0xE8, 0xE7, 0xF0))
{
    // Default window size
    QScreen* screen = QGuiApplication::primaryScreen();
    QSize availableSize = screen->availableSize();
    QRect availableGeometry = screen->availableGeometry();

    setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    availableSize * WINDOW_SIZE_RATIO,
                    availableGeometry
                    )
                );

    BaseQmlWidget* rootWidget = new BaseQmlWidget(this);
    rootWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    rootWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rootWidget->setSource(QUrl("qrc:/qml/Welcome/Welcome.qml"));
    setCentralWidget(rootWidget);
}
