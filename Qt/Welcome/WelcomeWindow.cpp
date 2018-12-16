#include "WelcomeWindow.h"

#include <QFontDatabase>
#include <QGraphicsEffect>
#include <QScreen>
#include <QLabel>
#include <QGuiApplication>
#include <QQmlContext>

#include "App/AppSettings.h"
#include "Project/ProjectWindow.h"
#include "Utils/BaseQmlWidget.h"
#include "App/VxAppUtils.h"

constexpr double WINDOW_SIZE_RATIO = 0.65;

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

    QQmlContext* context = rootWidget->rootContext();
    context->setContextProperty("self", this);

    QVariantList recordings {
    };
    context->setContextProperty("recordings", recordings);

    QVariantList projects {
    };
    context->setContextProperty("projects", projects);

    rootWidget->setSource(QUrl("qrc:/qml/Welcome/Welcome.qml"));
    setCentralWidget(rootWidget);
}

void WelcomeWindow::openExistingProject() {
    hide();
    if (VxAppUtils::OpenExistingProject(this)) {
        auto *projectWindow = new ProjectWindow();
        projectWindow->setAttribute(Qt::WA_DeleteOnClose, true);
        projectWindow->showMaximized();
    } else {
        show();
    }
}
