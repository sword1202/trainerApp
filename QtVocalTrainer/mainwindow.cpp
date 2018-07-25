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

constexpr int HEADER_HEIGHT = 75 + 61 - (int)WorkspaceDrawer::YARD_STICK_HEIGHT;
constexpr int PIANO_WIDTH = WorkspaceDrawer::PIANO_WIDTH;
constexpr int PLAY_HEAD_SIZE = 11;
constexpr int BEATS_IN_TACT = 4;

using namespace CppUtils;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent)
{
#ifdef __APPLE__
    doMacOsPlatformStaff();
#endif
    setupWorkspaceView();
    workspaceView->move(0, HEADER_HEIGHT);
    workspaceView->resize(1024, 768);

    cpp = new QmlCppBridge(this);

    // setup header
    QQuickWidget *headerWidget = createQQuickWidget("qrc:/qml/HeaderWithSubHeader.qml");
    header = headerWidget->rootObject();

    setupPlayHeadWidgets(&playHeadTriangle, &playHeadLine);
    setupPlayHeadWidgets(&playHeadTriangle2, &playHeadLine2);
    setBoundsSelectionEnabled(false);

    VxApp::instance()->getZoomController()->addZoomChangedListener([=] (float zoom) {
        updatePlayHeadPosition();
        return DONT_DELETE_LISTENER;
    });

    updatePlayHeadPosition();
    setPlayHeadPosition(300, 1);
    setupMenus();

    workspaceView->onClick = [=] (QMouseEvent* event) {
        onWorkspaceClick(event);
    };

    MvxPlayer *player = MainController::instance()->getPlayer();
    player->addPlayRequestedListener([=] {
        movePlayHeadToPlaybackStart();
        return DONT_DELETE_LISTENER;
    });
}

void MainWindow::setupPlayHeadWidgets(QSvgWidget** playHeadTriangle, QFrame** playHeadLine) {
    *playHeadTriangle = new QSvgWidget(":/qml/images/play_head_triangle.svg", this);
    (*playHeadTriangle)->resize(PLAY_HEAD_SIZE, PLAY_HEAD_SIZE);

    *playHeadLine = QtUtils::createVerticalLine(1, this);
    (*playHeadLine)->setStyleSheet(QString("background-color: #24232D;"));
}

void MainWindow::movePlayHeadToPlaybackStart() {
    MvxPlayer* player = MainController::instance()->getPlayer();
    player->setSeek(player->getSeek() + player->getTactDuration() * (playHeadOffsetFactor - 1.0));
    playHeadOffsetFactor = 1.0;
    updatePlayHeadPosition();
}

void MainWindow::onWorkspaceClick(QMouseEvent *event) {
    int minimumOffset = getMinimumPlayHeadOffset();

    if (event->button() == Qt::LeftButton) {
        int position = std::max(minimumOffset, event->x());
        playHeadOffsetFactor = (double)position / minimumOffset;
        if (MainController::instance()->getPlayer()->isPlaying()) {
            movePlayHeadToPlaybackStart();
        } else {
            setPlayHeadPosition(position, 0);
        }
    }
}

void MainWindow::updatePlayHeadPosition() {
    MainController* instance = MainController::instance();
    if (instance->getPlayer()->isPlaying()) {
        setPlayHeadPosition(getMinimumPlayHeadOffset(), 0);
    } else {
        setPlayHeadPosition(qRound(getMinimumPlayHeadOffsetF() * playHeadOffsetFactor), 0);
    }
}

int MainWindow::getMinimumPlayHeadOffset() const {
    return qRound(getMinimumPlayHeadOffsetF());
}

float MainWindow::getMinimumPlayHeadOffsetF() const {
    return MainController::instance()->getZoomController()->getIntervalWidth() * BEATS_IN_TACT;
}

void MainWindow::setPlayHeadPosition(int position, int index) {
    QWidget* playHeadTriangle = index == 0 ? this->playHeadTriangle : playHeadTriangle2;
    QWidget* playHeadLine = index == 0 ? this->playHeadLine : playHeadLine2;

    QRect geometry = playHeadTriangle->geometry();
    QPoint move(PIANO_WIDTH + position, HEADER_HEIGHT + (int)WorkspaceDrawer::YARD_STICK_HEIGHT);
    geometry.moveCenter(move);
    playHeadTriangle->setGeometry(geometry);
    playHeadLine->move(move);
}

int MainWindow::getPlayHeadPosition(int index) const {
    QWidget* playHeadLine = index == 0 ? this->playHeadLine : playHeadLine2;
    return playHeadLine->x() - PIANO_WIDTH;
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

    resizePlayHeadLine(0);
    resizePlayHeadLine(1);
}

void MainWindow::resizePlayHeadLine(int index) {
    QWidget* playHeadLine = index == 0 ? this->playHeadLine : playHeadLine2;
    playHeadLine->resize(playHeadLine->width(), workspaceView->height() - (int) WorkspaceDrawer::YARD_STICK_HEIGHT);
}

void MainWindow::setupWorkspaceView() {
    workspaceView = new QOpenGLWorkspaceWidget(this);
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
    if (!fileName.isNull()) {
        VxApp::instance()->getPlayer()->setSource(fileName);
    }
}

void MainWindow::setBoundsSelectionEnabled(bool enabled) {
    playHeadLine2->setVisible(enabled);
    playHeadTriangle2->setVisible(enabled);
}

MainWindow::~MainWindow()
{

}