#include "WelcomeWindow.h"

#include <QFontDatabase>
#include <QGraphicsEffect>
#include <QScreen>
#include <QFileInfo>
#include <QLabel>

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
    effect->setBlurRadius(40);
    effect->setOffset(0, 0);
    effect->setColor(QColor("#615F9722"));
    ui->bottomWidget->setGraphicsEffect(effect);

    // Load projects (temporary use this values)
    AppSettings settings;
    settings.setProjects({"/mnt/Storage/Музыка/Metallica/Metallica – Enter Sandman.mp3", "/mnt/Storage/Музыка/Metallica/Metallica – Turn The Page.mp3"});
    settings.setRecordings({"/mnt/Storage/Музыка/Carl Perkins/Carl Perkins – All Shook Up.mp3", "/mnt/Storage/Музыка/Metallica/Metallica – Turn The Page.mp3", "/mnt/Storage/Музыка/Frank Sinatra/Frank Sinatra – Let It Snow! Let It Snow! Let It Snow!.mp3"});

    // Check if no recordings
    QStringList recrodings = settings.getRecordings();
    if (recrodings.isEmpty()) {
        // Show no recordings info
        ui->recordingsPageLayout->removeWidget(ui->recordingsList);
        ui->recordingsPageLayout->setContentsMargins(0, 91, 0, 0);

        auto imageLabel = new QLabel(this);
        imageLabel->setPixmap(QPixmap(":/icons/no_recordings.svg"));
        imageLabel->setAlignment(Qt::AlignHCenter);
        imageLabel->setMargin(12);
        ui->recordingsPageLayout->addWidget(imageLabel);

        auto titleLabel = new QLabel(this);
        titleLabel->setText("You don’t have any recordings yet");
        titleLabel->setFont(QFont("Lato", 13));
        titleLabel->setStyleSheet("color: rgb(36, 35, 45);");
        titleLabel->setAlignment(Qt::AlignHCenter);
        titleLabel->setMargin(13);
        ui->recordingsPageLayout->addWidget(titleLabel);

        auto textLabel = new QLabel(this);
        textLabel->setText("Your recordings appear here.\n"
                           "To start recording you can either use one of the existing projects in the “All Projects” or create your own in the “New Project”.");
        textLabel->setFont(QFont("Lato", 11));
        textLabel->setStyleSheet("color: rgb(81, 78, 100);");
        textLabel->setAlignment(Qt::AlignHCenter);
        textLabel->setWordWrap(true);
        textLabel->setMinimumWidth(450);
        ui->recordingsPageLayout->addWidget(textLabel);
        ui->recordingsPageLayout->setAlignment(textLabel, Qt::AlignHCenter);

        ui->recordingsPageLayout->setRowStretch(3, 1);
    } else {
        addItems(recrodings, ProjectItem::Record);
    }

    addItems({"New Project"}, ProjectItem::NewProject);
    addItems(settings.getProjects(), ProjectItem::Project);
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
