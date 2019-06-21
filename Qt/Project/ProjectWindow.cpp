#include "ProjectWindow.h"

#include <QOpenGLWidget>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QQuickItem>
#include <QQmlContext>
#include <QScreen>
#include <QFileDialog>
#include <QMenuBar>
#include <QMacCocoaViewContainer>
#include <QSizePolicy>
#include <QPushButton>

#include "QtBridge/QtCppBridge.h"
#include "App/VxApp.h"
#include "PlaybackBounds.h"
#include "Algorithms.h"
#include "PortAudioUtils.h"
#include "SelectMicrophoneDialog.h"
#include "AddLyricsDialog.h"
#include "App/AppSettings.h"
#include <QScrollBar>
#include <NotImplementedAssert.h>
#include <Executors.h>

#include "App/VxAppUtils.h"
#include "SingingResultDialog.h"

#include "Utils/BaseQmlWidget.h"

constexpr int YARD_STICK_HEIGHT = static_cast<int>(WorkspaceDrawer::YARD_STICK_HEIGHT);
constexpr int HEADER_WITH_SUBHEADER_HEIGHT = 75 + 75 - YARD_STICK_HEIGHT;
constexpr int VERTICAL_SCROLL_WIDTH = 11;
constexpr int BEATS_IN_TACT = 4;
constexpr int MINIMUM_WINDOW_WIDTH = 700;
constexpr double MINIMUM_WINDOW_HEIGHT_RATIO = 0.6;
constexpr int LYRICS_HEIGHT = 53;
constexpr int LYRICS_UPDATE_INTERVAL = 50;

#ifdef __APPLE__
#define IS_APPLE true
#else
#define IS_APPLE false
#endif

using namespace CppUtils;
using std::cout;
using std::endl;

ProjectWindow::ProjectWindow() :
        BaseMainWindow(QColor::fromRgb(197, 206, 248))
{
    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    AppSettings settings;
    float inputVolume = settings.getInputVolume();
    float outputVolume = settings.getOutputVolume();
    float recordingVolume = settings.getRecordingVolume();
    
    // Workspace
#ifdef __APPLE__
    workspaceWidget = new MetalWorkspaceWidget(this);
#else
    workspaceWidget = new OpenGLWorkspaceWidget(this);
#endif
    workspaceWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setupVolumeWidget();

    // Lyrics
    lyricsWidget = createQQuickWidget("qrc:/qml/Project/Lyrics.qml", [] (QQmlContext* context) {
        context->setContextProperty("lyricsText", "");
    });
    lyricsWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    lyricsWidget->setFixedHeight(LYRICS_HEIGHT);
    lyricsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Header
    QQuickWidget *headerWidget = createQQuickWidget("qrc:/qml/Project/Header/HeaderWithSubHeader.qml");
    headerWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    headerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    headerWidget->setFixedHeight(HEADER_WITH_SUBHEADER_HEIGHT);
    headerWithSubheader = headerWidget->rootObject();
    QQmlContext *context = headerWidget->rootContext();
    context->setContextProperty("minZoom", WorkspaceDrawer::MIN_ZOOM);
    context->setContextProperty("maxZoom", WorkspaceDrawer::MAX_ZOOM);
    header = headerWithSubheader->findChild<QQuickItem*>("header");
    assert(header);

    MainController *mainController = MainController::instance();
    MvxPlayer *player = mainController->getPlayer();
    AudioInputManager *audioInputManager = mainController->getAudioInputManager();

    if (player->isRecording()) {
        header->setProperty("recordingVolume", recordingVolume);
    } else {
        header->setProperty("inputVolume", inputVolume);
        onInputVolumeChanged(inputVolume);

        header->setProperty("outputVolume", outputVolume);
        onOutputVolumeChanged(outputVolume);
    }

    auto audioLevelListener = [=] (double level) {
        header->setProperty("microphoneLevel", level);
    };
    if (player->isRecording()) {
        player->recordingVoiceLevelListeners.addListener(audioLevelListener);
    } else {
        audioInputManager->addAudioInputLevelMonitor(audioLevelListener);
    }

    // Setup layouts
    auto *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(workspaceWidget);
    mainLayout->addWidget(lyricsWidget);
    centralWidget->setLayout(mainLayout);

    setupMenus();

    if (!player->isRecording()) {
        player->onCompleteListeners.addListener([=] {
            auto* dialog = new SingingResultDialog(this);
            dialog->show();
        });
    }

    MainController::instance()->getWorkspaceController([this] (WorkspaceController* workspaceController) {
        this->workspaceController = workspaceController;
    });


    bool hasLyrics = player->hasLyrics();
    setShowLyrics(hasLyrics);
    QtUtils::StartRepeatedTimer(this, [=] {
        if (player->getLyricsLinesCount() <= 0) {
            return true;
        }

        const std::string& lyricsTextUtf8 = player->getLyricsTextAtLine(0);
        lyricsWidget->rootContext()->setContextProperty("lyricsText", QtUtils::QStringFromUtf8(lyricsTextUtf8));
        return true;
    }, LYRICS_UPDATE_INTERVAL);
}

void ProjectWindow::setupVolumeWidget() {
    QQuickWidget* volumeWidget = new BaseQmlWidget(this);
    volumeWidget->setSource(QUrl("qrc:/qml/Project/TracksVolumeController.qml"));
    auto* wrapper = workspaceWidget->addSubWidget(volumeWidget);

    QtUtils::AddResizeListener(workspaceWidget, [=] (int width, int height) {
        int y = height - volumeWidget->height();
        wrapper->move(0, y);
    });

    this->volumeWidget = wrapper;
}

void ProjectWindow::onOutputVolumeChanged(float value) {
    AppSettings settings;
    settings.setOutputVolume(value);
    AudioInputManager *audioInputManager = MainController::instance()->getAudioInputManager();
    audioInputManager->setOutputVolume(value);
}

void ProjectWindow::onInputVolumeChanged(float value) {
    AppSettings settings;
    settings.setInputVolume(value);
    AudioInputManager *audioInputManager = MainController::instance()->getAudioInputManager();
    audioInputManager->setInputVolume(value);
}

void ProjectWindow::onRecordingVolumeChanged(float value) {
    AppSettings settings;
    settings.setRecordingVolume(value);
    MainController::instance()->getPlayer()->setRecordingVolume(value);
}

void ProjectWindow::setupMenus() {
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* openAction = fileMenu->addAction("Open...");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &ProjectWindow::onFileOpen);

    QMenu* editMenu = menuBar()->addMenu("Edit");
    QAction* microphoneAction = editMenu->addAction("Select Microphone");
    connect(microphoneAction, &QAction::triggered, this, &ProjectWindow::onSelectMicrophone);

    QAction* addLyricsAction = editMenu->addAction("Add Lyrics");
    connect(addLyricsAction, &QAction::triggered, this, &ProjectWindow::onAddLyrics);
    MvxPlayer *player = MainController::instance()->getPlayer();
    addLyricsAction->setVisible(player->getBounds());
    player->boundsChangedListeners.addListener([=] (const PlaybackBounds& bounds) {
        addLyricsAction->setVisible(bounds);
    });
}

void ProjectWindow::onFileOpen() {
    VxAppUtils::OpenExistingProject(this);
}

void ProjectWindow::onSelectMicrophone() {
    (new SelectMicrophoneDialog(this, cpp))->show();
}

void ProjectWindow::onAddLyrics() {
    (new AddLyricsDialog(this, cpp))->show();
}

void ProjectWindow::setBoundsSelectionEnabled(bool enabled) {
    MainController::instance()->getPlaybackBoundsSelectionController()->setBoundsSelectionEnabled(enabled);
}

void ProjectWindow::setShowLyrics(bool value) {
    lyricsWidget->setVisible(value);
}

void ProjectWindow::setShowTracks(bool value) {
    MainController::instance()->getWorkspaceController([=] (WorkspaceController* workspaceController) {
        workspaceController->setDrawTracks(value);
    });
    volumeWidget->setVisible(value);
}

void ProjectWindow::wheelEvent(QWheelEvent *event) {
    QWidget::wheelEvent(event);
    QPoint delta = event->pixelDelta();
    int y = -delta.y();
    float maxDelta = workspaceController->getVisibleGridHeight();
    float positionDelta = y / maxDelta;
    float newPosition = workspaceController->getVerticalScrollPosition() + positionDelta;
    if (newPosition < 0) {
        newPosition = 0;
    } else if(newPosition > 1) {
        newPosition = 1;
    }

    workspaceController->setVerticalScrollPosition(newPosition);
}

float ProjectWindow::getZoom() const {
    return workspaceController ? workspaceController->getZoom() : WorkspaceDrawer::MIN_ZOOM;
}

void ProjectWindow::setZoom(float zoom) {
    MainController::instance()->getWorkspaceController([=] (WorkspaceController* workspaceController) {
        workspaceController->setZoom(zoom);
        emit zoomChanged();
    });
}

