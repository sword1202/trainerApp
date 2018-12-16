#include "WelcomeWindow.h"

#include <QFontDatabase>
#include <QGraphicsEffect>
#include <QScreen>
#include <QLabel>
#include <QGuiApplication>
#include <QQmlContext>

#include "App/AppSettings.h"

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

    QVariantList recordings {
            QVariantMap {
                {"title", "Fear of the Dark"},
                {"artistName", "Iron Maiden"},
                {"score", 57}
            },
            QVariantMap {
                {"title", "Send Me An Angel"},
                {"artistName", "Scorpions"},
                {"score", 98}
            },
            QVariantMap {
                {"title", "Sky Is Over"},
                {"artistName", "Serj Tankian"},
                {"score", 67}
            }
    };
    context->setContextProperty("recordings", recordings);

    QVariantList projects {
            QVariantMap {
                    {"title", "Fear of the Dark"},
                    {"artistName", "Iron Maiden"},
            },
            QVariantMap {
                    {"title", "Send Me An Angel"},
                    {"artistName", "Scorpions"},
            },
            QVariantMap {
                    {"title", "Sky Is Over"},
                    {"artistName", "Serj Tankian"}
            }
    };
    context->setContextProperty("projects", projects);

    rootWidget->setSource(QUrl("qrc:/qml/Welcome/Welcome.qml"));
    setCentralWidget(rootWidget);
}
