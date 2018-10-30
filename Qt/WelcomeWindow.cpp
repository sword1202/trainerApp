#include "WelcomeWindow.h"

#include <QFontDatabase>
#include <QGraphicsEffect>
#include <QScreen>
#include <QFileInfo>

#include "ui_WelcomeWindow.h"
#include "AppSettings.h"

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
    connect(ui->leftListWidget, &QListWidget::currentRowChanged, ui->stackedWidget, &QStackedWidget::setCurrentIndex);

    // Bottom shadow
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(60);
    effect->setOffset(0, 0);
    effect->setColor(QColor(100, 100, 100, 56));
    ui->bottomWidget->setGraphicsEffect(effect);

    // Load projects (temporary use this values)
    AppSettings settings;
    settings.setProjects({"/mnt/Storage/Музыка/Metallica/Metallica – Enter Sandman.mp3", "/mnt/Storage/Музыка/Metallica/Metallica – Turn The Page.mp3"});
    settings.setRecordings({"/mnt/Storage/Музыка/Carl Perkins/Carl Perkins – All Shook Up.mp3", "/mnt/Storage/Музыка/Metallica/Metallica – Turn The Page.mp3", "/mnt/Storage/Музыка/Frank Sinatra/Frank Sinatra – Let It Snow! Let It Snow! Let It Snow!.mp3"});

    addItems({"New Project"}, ProjectItem::NewProject);
    addItems(settings.getProjects(), ProjectItem::Project);
    addItems(settings.getRecordings(), ProjectItem::Record);
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}

void WelcomeWindow::addItems(const QStringList &list, ProjectItem::Type type)
{
    // Select list widget to add
    QListWidget *widgetList;
    switch (type) {
    case ProjectItem::NewProject:
        widgetList = ui->newProjectList;
        break;
    case ProjectItem::Project:
        widgetList = ui->allProjectsList;
        break;
    case ProjectItem::Record:
        widgetList = ui->recordingsList;
        break;
    }

    for (auto name : list) {
        auto projectItem = new ProjectItem(name, type, this);
        auto widgetItem = new QListWidgetItem(widgetList);
        widgetItem->setSizeHint(projectItem->sizeHint());
        widgetList->setItemWidget(widgetItem, projectItem);
    }
}
