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
    setupProjectsList(context);

    rootWidget->setSource(QUrl("qrc:/qml/Welcome/Welcome.qml"));
    setCentralWidget(rootWidget);
}

void WelcomeWindow::setupProjectsList(QQmlContext *context) {
    QVariantList recordings, projects;
    AppSettings appSettings;
    QStringList filePaths;
    filePaths << appSettings.getProjects() << appSettings.getRecordings();
    for (const QString &filePath : filePaths) {
        // read only signature
        MvxFile file;
        try {
            file = MvxFile::readFromFile(filePath.toLocal8Bit(), true);
        } catch (...) {
            appSettings.removeProject(filePath);
            appSettings.removeRecording(filePath);
            return;
        }


        QVariantMap map;
        map["artistName"] = QtUtils::QStringFromUtf8(file.getArtistNameUtf8());
        map["title"] = QtUtils::QStringFromUtf8(file.getSongTitleUtf8());
        map["filePath"] = filePath;

        if (file.isRecording()) {
            map["score"] = file.getScore();
            recordings.append(map);
        } else {
            projects.append(map);
        }
    }

    context->setContextProperty("recordings", recordings);
    context->setContextProperty("projects", projects);
}

void WelcomeWindow::openExistingProject() {
    hide();
    if (VxAppUtils::OpenExistingProject(this)) {
        showProjectWindow();
    } else {
        show();
    }
}

void WelcomeWindow::openRecentProject(const QString& filePath) {
    hide();
    VxAppUtils::OpenProject(filePath);
    showProjectWindow();
}

void WelcomeWindow::showProjectWindow() {
    auto *projectWindow = new ProjectWindow();
    projectWindow->setAttribute(Qt::WA_DeleteOnClose, true);
    projectWindow->showMaximized();
}
