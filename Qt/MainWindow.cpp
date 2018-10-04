#include "MainWindow.h"

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

#include "QmlCppBridge.h"
#include "VxApp.h"
#include "QOpenglWorkspaceWidget.h"
#include "PlaybackBounds.h"
#include "QPainterWorkspaceWidget.h"
#include "Algorithms.h"
#include "PortAudioUtils.h"
#include "SelectMicrophoneDialog.h"
#include "AppSettings.h"

constexpr int YARD_STICK_HEIGHT = static_cast<int>(WorkspaceDrawer::YARD_STICK_HEIGHT);
constexpr int HEADER_WITH_SUBHEADER_HEIGHT = 75 + 75 - YARD_STICK_HEIGHT;
constexpr int VERTICAL_SCROLL_WIDTH = 11;
constexpr int BEATS_IN_TACT = 4;
constexpr int MINIMUM_WINDOW_WIDTH = 700;
constexpr double MINIMUM_WINDOW_HEIGHT_RATIO = 0.6;
constexpr int LYRICS_HEIGHT = 53;

using namespace CppUtils;
using std::cout;
using std::endl;

MainWindow::MainWindow() :
        BaseMainWindow(QColor::fromRgb(197, 206, 248))
{
    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    AppSettings settings;
    float inputVolume = settings.getInputVolume();
    float outputVolume = settings.getOutputVolume();
    
    // Workspace
    workspaceView = new QOpenGLWorkspaceWidget(this);
    workspaceView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Lyrics
    lyricsWidget = createQQuickWidget("qrc:/qml/Lyrics.qml");
    lyricsWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    lyricsWidget->setFixedHeight(LYRICS_HEIGHT);
    lyricsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Header
    QQuickWidget *headerWidget = createQQuickWidget("qrc:/qml/HeaderWithSubHeader.qml");
    headerWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    headerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    headerWidget->setFixedHeight(HEADER_WITH_SUBHEADER_HEIGHT);
    headerWithSubheader = headerWidget->rootObject();
    header = headerWithSubheader->findChild<QQuickItem*>("header");
    assert(header);

    AudioInputManager *audioInputManager = MainController::instance()->getAudioInputManager();
    
    header->setProperty("inputVolume", inputVolume);
    onInputVolumeChanged(inputVolume);
    
    header->setProperty("outputVolume", outputVolume);
    onOutputVolumeChanged(outputVolume);
    
    audioInputManager->addAudioInputLevelMonitor([=] (double level) {
        header->setProperty("microphoneLevel", level);
    });

    // Setup layouts
    auto *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(workspaceView);
    mainLayout->addWidget(lyricsWidget);
    centralWidget->setLayout(mainLayout);

	// Scrollbar
    verticalScrollWidget = createQQuickWidget("qrc:/qml/VerticalScrollBarContainer.qml");
	verticalScrollWidget->rootObject()->setWidth(VERTICAL_SCROLL_WIDTH);

    setupMenus();
}

void MainWindow::onOutputVolumeChanged(float value) {
    AppSettings settings;
    settings.setOutputVolume(value);
    AudioInputManager *audioInputManager = MainController::instance()->getAudioInputManager();
    audioInputManager->setOutputVolume(value);
}

void MainWindow::onInputVolumeChanged(float value) {
    AppSettings settings;
    settings.setInputVolume(value);
    AudioInputManager *audioInputManager = MainController::instance()->getAudioInputManager();
    audioInputManager->setInputVolume(value);
}

int MainWindow::getMinimumPlayHeadOffset() const {
    return qRound(getMinimumPlayHeadOffsetF());
}

float MainWindow::getMinimumPlayHeadOffsetF() const {
    return MainController::instance()->getZoomController()->getIntervalWidth() * BEATS_IN_TACT;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    const int width = event->size().width();
    const int height = event->size().height();

    verticalScrollWidget->rootObject()->setHeight(workspaceView->height() - YARD_STICK_HEIGHT - 1);
	verticalScrollWidget->move(width - VERTICAL_SCROLL_WIDTH, workspaceView->y() + YARD_STICK_HEIGHT + 1);
}

void MainWindow::setupMenus() {
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* openAction = fileMenu->addAction("Open...");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::onFileOpen);

    QMenu* editMenu = menuBar()->addMenu("Edit");
    QAction* microphoneAction = editMenu->addAction("Select Microphone");
    connect(microphoneAction, &QAction::triggered, this, &MainWindow::onSelectMicrophone);
}

void MainWindow::onFileOpen() {
    QString fileName = QFileDialog::getOpenFileName(
            this, "Select .mvx file for signing", "", "Mvx files(*.mvx);; All files(*)");

    //QString fileName = "/Users/Semyon/Downloads/torero.mvx";
    if (!fileName.isEmpty()) {
        VxApp::instance()->getPlayer()->setSource(fileName);
    }
}

void MainWindow::onSelectMicrophone() {
    SelectMicrophoneDialog* dialog = new SelectMicrophoneDialog(this, cpp);
    dialog->show();
}

void MainWindow::setBoundsSelectionEnabled(bool enabled) {
    MainController::instance()->getPlaybackBoundsSelectionController()->setBoundsSelectionEnabled(enabled);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setShowLyrics(bool value) {
    lyricsWidget->setVisible(value);
}
