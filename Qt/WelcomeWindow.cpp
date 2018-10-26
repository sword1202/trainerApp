#include "WelcomeWindow.h"

#include <QFontDatabase>
#include <QGraphicsEffect>
#include <QScreen>

#include "ui_WelcomeWindow.h"

constexpr double WINDOW_SIZE_RATIO = 0.8;

WelcomeWindow::WelcomeWindow() :
    BaseMainWindow(QColor::fromRgb(0xE8, 0xE7, 0xF0)),
    ui(new Ui::WelcomeWindow)
{
    ui->setupUi(this);

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

    // Pages selection mechanism
    connect(ui->listWidget, &QListWidget::currentRowChanged, ui->stackedWidget, &QStackedWidget::setCurrentIndex);

    // Bottom shadow
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(40);
    effect->setOffset(0, 0);
    effect->setColor(QColor(100, 100, 100, 127));
    ui->bottomWidget->setGraphicsEffect(effect);
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}
