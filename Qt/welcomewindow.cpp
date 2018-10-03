#include "welcomewindow.h"
#include <QVBoxLayout>
#include <QQuickItem>
#include <QGuiApplication>
#include <QScreen>
#include <QStyle>

constexpr double WINDOW_SIZE_RATIO = 0.8;

WelcomeWindow::WelcomeWindow() : BaseMainWindow(QColor::fromRgb(0xE8, 0xE7, 0xF0)) {
    widget = createQQuickWidget("qrc:/qml/WelcomeWindow.qml");
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

    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(widget);
    centralWidget->setLayout(mainLayout);
}

void WelcomeWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    int width = event->size().width();
    int height = event->size().height();
    QQuickItem *root = widget->rootObject();
    root->setWidth(width);
    root->setHeight(height);
}
