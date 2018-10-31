#include "WelcomeWindow.h"

#include <QFontDatabase>
#include <QGraphicsEffect>
#include <QScreen>
#include <QLabel>

#include "ui_WelcomeWindow.h"
#include "../AppSettings.h"

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
    effect->setBlurRadius(40);
    effect->setOffset(0, 0);
    effect->setColor(QColor("#615F9722"));
    ui->bottomWidget->setGraphicsEffect(effect);

    // Load projects (temporary use this values)
    AppSettings settings;
    settings.setProjects({"/mnt/Storage/Metallica/Metallica – Enter Sandman.mp3", "/mnt/Storage/Metallica/Metallica – Turn The Page.mp3"});
    settings.setRecordings({"/mnt/Storage/Carl Perkins/Carl Perkins – All Shook Up.mp3", "/mnt/Storage/Metallica/Metallica – Turn The Page.mp3", "/mnt/Storage/Frank Sinatra/Frank Sinatra – Let It Snow! Let It Snow! Let It Snow!.mp3"});

    QStringList recrodings = settings.getRecordings();
    if (recrodings.isEmpty())
        showNoItemsInfo(ui->recordingsList, ui->recordingsPageLayout);
    else
        addItems(recrodings, ProjectItem::RECORD);

    QStringList projects = settings.getProjects();
    if (recrodings.isEmpty())
        showNoItemsInfo(ui->allProjectsList, ui->allProjectsPageLayout);
    else
        addItems(settings.getProjects(), ProjectItem::PROJECT);

    addItems({"New Project"}, ProjectItem::NEW_PROJECT);
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
    case ProjectItem::NEW_PROJECT:
        widgetList = ui->newProjectList;
        break;
    case ProjectItem::PROJECT:
        widgetList = ui->allProjectsList;
        break;
    case ProjectItem::RECORD:
        widgetList = ui->recordingsList;
        break;
    }

    for (const auto &name : list) {
        auto *projectItem = new ProjectItem(name, type, this);
        auto *widgetItem = new QListWidgetItem(widgetList);
        widgetItem->setSizeHint(projectItem->sizeHint());
        widgetList->setItemWidget(widgetItem, projectItem);
    }
}

void WelcomeWindow::showNoItemsInfo(QListWidget *widgetList, QGridLayout *layout)
{
    layout->removeWidget(widgetList);
    layout->setContentsMargins(0, 91, 0, 0); // Add top indent for image

    auto *noItemsImage = new QLabel(this);
    noItemsImage->setPixmap(QPixmap("://Resources/Welcome/no_recordings.svg"));
    noItemsImage->setAlignment(Qt::AlignHCenter);
    noItemsImage->setMargin(12);
    layout->addWidget(noItemsImage);

    auto *noItemsTitle = new QLabel(this);
    noItemsTitle->setText("You don’t have any recordings yet");
    noItemsTitle->setFont(QFont("Lato", 13));
    noItemsTitle->setStyleSheet("color: rgb(81, 78, 100);"); // Set text color
    noItemsTitle->setAlignment(Qt::AlignHCenter);
    noItemsTitle->setMargin(13);
    layout->addWidget(noItemsTitle);

    auto *noItemsText = new QLabel(this);
    noItemsText->setText("Your recordings appear here.\n"
                       "To start recording you can either use one of the existing projects in the “All Projects” or create your own in the “New Project”.");
    noItemsText->setFont(QFont("Lato", 11));
    noItemsText->setStyleSheet("color: rgb(81, 78, 100);"); // Set text color
    noItemsText->setAlignment(Qt::AlignHCenter);
    noItemsText->setWordWrap(true);
    noItemsText->setMinimumWidth(450); // Set the minimum size to avoid flattening
    layout->addWidget(noItemsText);
    layout->setAlignment(noItemsText, Qt::AlignHCenter);

    layout->setRowStretch(3, 1);
}
