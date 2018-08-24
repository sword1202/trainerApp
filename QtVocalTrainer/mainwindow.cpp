#include "mainwindow.h"
#include <QOpenGLWidget>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QQuickItem>
#include <QQmlContext>
#include "QmlCppBridge.h"
#include "app.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QMacCocoaViewContainer>
#include <QThread>
#include "qopenglworkspacewidget.h"
#include "PlaybackBounds.h"
#include "qpainterworkspacewidget.h"
#include <iostream>

constexpr int YARD_STICK_HEIGHT = (int)WorkspaceDrawer::YARD_STICK_HEIGHT;
constexpr int HEADER_HEIGHT = 75 + 61 - YARD_STICK_HEIGHT;
constexpr int PIANO_WIDTH = WorkspaceDrawer::PIANO_WIDTH;
constexpr int PLAY_HEAD_SIZE = 11;
constexpr int VERTICAL_SCROLL_WIDTH = 11;
constexpr int BEATS_IN_TACT = 4;

using namespace CppUtils;
using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent)
{
#ifdef __APPLE__
    doMacOsPlatformStaff();
#endif

    setMinimumSize(1280, 1024);
    workspaceView = new QOpenGLWorkspaceWidget(this);
    workspaceView->move(0, HEADER_HEIGHT);
    //workspaceView->resize(1024, 768);

    cpp = new QmlCppBridge(this);

    // setup header
    QQuickWidget *headerWidget = createQQuickWidget("qrc:/qml/HeaderWithSubHeader.qml");
    header = headerWidget->rootObject();

    verticalScrollWidget = createQQuickWidget("qrc:/qml/VerticalScrollBarContainer.qml");
    verticalScroll = verticalScrollWidget->rootObject();

    setBoundsSelectionEnabled(false);

    setupMenus();
}

int MainWindow::getMinimumPlayHeadOffset() const {
    return qRound(getMinimumPlayHeadOffsetF());
}

float MainWindow::getMinimumPlayHeadOffsetF() const {
    return MainController::instance()->getZoomController()->getIntervalWidth() * BEATS_IN_TACT;
}

QQuickWidget *MainWindow::createQQuickWidget(const QString& qmlFile) {
    QQuickWidget* qmlWidget = new QQuickWidget(this);
    qmlWidget->rootContext()->setContextProperty("cpp", cpp);
    qmlWidget->setSource(QUrl(qmlFile));
    return qmlWidget;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    int width = event->size().width();
    int height = event->size().height();

    workspaceView->resize(width, height - HEADER_HEIGHT);

    header->setWidth(width);
    header->setHeight(HEADER_HEIGHT);

    verticalScroll->setWidth(VERTICAL_SCROLL_WIDTH);
    verticalScroll->setHeight(height - HEADER_HEIGHT - YARD_STICK_HEIGHT);
    verticalScrollWidget->move(width - VERTICAL_SCROLL_WIDTH, HEADER_HEIGHT + YARD_STICK_HEIGHT + 1);
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
}

void MainWindow::onFileOpen() {
    QString fileName = QFileDialog::getOpenFileName(
            this, "Select .mvx file for signing", "", "Mvx files(*.mvx);; All files(*)");

//    QString fileName = "C:/projects/projects_2018/vocal/torero.mvx";
    if (!fileName.isEmpty()) {
        VxApp::instance()->getPlayer()->setSource(fileName);
    }
}

void MainWindow::setBoundsSelectionEnabled(bool enabled) {
    boundsSelectionRunning = enabled;
}

MainWindow::~MainWindow()
{

}
