#include "mainwindow.h"
#include <QOpenGLWidget>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QQuickItem>
#include <QQmlContext>
#include "QmlCppBridge.h"
#include <QMenuBar>
#include <QFileDialog>

constexpr int HEADER_HEIGHT = 75 + 61;
constexpr int PIANO_WIDTH = 67;
constexpr int PLAY_HEAD_SIZE = 11;
constexpr int BEATS_IN_TACT = 4;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent)
{
#ifdef __APPLE__
    doMacOsPlatformStaff();
#endif

    // setup workspace
    workspace = new Workspace(this);
    workspace->move(PIANO_WIDTH, HEADER_HEIGHT);

    cpp = new QmlCppBridge(this);

    QQuickWidget* pianoWidget = createQQuickWidget("qrc:/qml/Piano.qml");
    pianoWidget->move(0, HEADER_HEIGHT);
    piano = pianoWidget->rootObject();

    // setup header
    QQuickWidget *headerWidget = createQQuickWidget("qrc:/qml/HeaderWithSubHeader.qml");
    header = headerWidget->rootObject();

    playHeadTriangle = new QSvgWidget(":/qml/images/play_head_triangle.svg", this);
    playHeadTriangle->resize(PLAY_HEAD_SIZE, PLAY_HEAD_SIZE);

    playHeadLine = QtUtils::createVerticalLine(1, this);
    playHeadLine->setStyleSheet(QString("background-color: #24232D;"));

    setPlayHeadPosition(qRound(ZoomController::instance()->getIntervalWidth() * BEATS_IN_TACT));

    setupMenus();
}

void MainWindow::setPlayHeadPosition(int position) const {
    QRect geometry = playHeadTriangle->geometry();
    QPoint move(PIANO_WIDTH + position, HEADER_HEIGHT);
    geometry.moveCenter(move);
    playHeadTriangle->setGeometry(geometry);
    playHeadLine->move(move);
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

    workspace->resize(width - PIANO_WIDTH, height - HEADER_HEIGHT);

    header->setWidth(width);
    header->setHeight(HEADER_HEIGHT);

    piano->setWidth(PIANO_WIDTH);
    piano->setHeight(height - HEADER_HEIGHT);

    playHeadLine->resize(playHeadLine->width(), workspace->height());
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
        Player::instance()->setSource(fileName);
    }
}

MainWindow::~MainWindow()
{

}
