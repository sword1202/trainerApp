#include "mainwindow.h"

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
#include "app.h"
#include "qopenglworkspacewidget.h"
#include "PlaybackBounds.h"
#include "qpainterworkspacewidget.h"
#include "Algorithms.h"
#include "PortAudioUtils.h"
#include "selectmicrophonedialog.h"

constexpr int YARD_STICK_HEIGHT = static_cast<int>(WorkspaceDrawer::YARD_STICK_HEIGHT);
constexpr int HEADER_HEIGHT = 75 + 61 - YARD_STICK_HEIGHT;
constexpr int VERTICAL_SCROLL_WIDTH = 11;
constexpr int BEATS_IN_TACT = 4;

using namespace CppUtils;
using std::cout;
using std::endl;

MainWindow::MainWindow() :
        BaseMainWindow(QColor::fromRgb(197, 206, 248))
{
    // Workspace
    workspaceView = new QOpenGLWorkspaceWidget(this);
    workspaceView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Header
    QQuickWidget *headerWidget = createQQuickWidget("qrc:/qml/HeaderWithSubHeader.qml");
    header = headerWidget->rootObject();
    header->setHeight(HEADER_HEIGHT);

    // Scrollbar
    verticalScrollWidget = createQQuickWidget("qrc:/qml/VerticalScrollBarContainer.qml");
    verticalScroll = verticalScrollWidget->rootObject();
    verticalScroll->setWidth(VERTICAL_SCROLL_WIDTH);

    // Setup layouts
    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(headerWidget);
    centralWidget->setLayout(mainLayout);

    QHBoxLayout *workspaceLayout = new QHBoxLayout;
    workspaceLayout->setMargin(0);
    workspaceLayout->setSpacing(0);
    workspaceLayout->addWidget(workspaceView);
    workspaceLayout->addWidget(verticalScrollWidget);
    mainLayout->addLayout(workspaceLayout);

    setupMenus();
}

int MainWindow::getMinimumPlayHeadOffset() const {
    return qRound(getMinimumPlayHeadOffsetF());
}

float MainWindow::getMinimumPlayHeadOffsetF() const {
    return MainController::instance()->getZoomController()->getIntervalWidth() * BEATS_IN_TACT;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    int width = event->size().width();
    int height = event->size().height();

    header->setWidth(width);
    verticalScroll->setHeight(height - HEADER_HEIGHT - YARD_STICK_HEIGHT);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    //header->update();
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
//    QString fileName = QFileDialog::getOpenFileName(
//            this, "Select .mvx file for signing", "", "Mvx files(*.mvx);; All files(*)");

    QString fileName = "/Users/Semyon/Downloads/torero.mvx";
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
