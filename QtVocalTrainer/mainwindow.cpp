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

#include "QmlCppBridge.h"
#include "app.h"
#include "qopenglworkspacewidget.h"
#include "PlaybackBounds.h"
#include "qpainterworkspacewidget.h"

constexpr int YARD_STICK_HEIGHT = static_cast<int>(WorkspaceDrawer::YARD_STICK_HEIGHT);
constexpr int HEADER_HEIGHT = 75 + 61 - YARD_STICK_HEIGHT;
constexpr int PIANO_WIDTH = WorkspaceDrawer::PIANO_WIDTH;
constexpr int PLAY_HEAD_SIZE = 11;
constexpr int VERTICAL_SCROLL_WIDTH = 11;
constexpr int BEATS_IN_TACT = 4;
constexpr int MINIMUM_WINDOW_WIDTH = 700;
constexpr int WORKSPACE_HEADER = 22;
constexpr double MINIMUM_WINDOW_HEIGHT_RATIO = 0.6;

using namespace CppUtils;
using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent)
{
#ifdef __APPLE__
    doMacOsPlatformStaff();
#endif

    // Window size
    QSize availableSize = QGuiApplication::primaryScreen()->availableSize();
    if (availableSize.width() < MINIMUM_WINDOW_WIDTH)
        setMinimumSize(MINIMUM_WINDOW_WIDTH, static_cast<int>(availableSize.height() * MINIMUM_WINDOW_HEIGHT_RATIO));
    else
        setMinimumSize(availableSize.height(), static_cast<int>(availableSize.height() * MINIMUM_WINDOW_HEIGHT_RATIO));

    // Workspace
    workspaceView = new QOpenGLWorkspaceWidget(this);
    workspaceView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    cpp = new QmlCppBridge(this);

    // Header
    QQuickWidget *headerWidget = createQQuickWidget("qrc:/qml/HeaderWithSubHeader.qml");
    header = headerWidget->rootObject();
    header->setHeight(HEADER_HEIGHT);

    // Setup layouts
    auto *centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(workspaceView);
    centralWidget->setLayout(mainLayout);

	// Scrollbar
	verticalScrollWidget = createQQuickWidget("qrc:/qml/VerticalScrollBarContainer.qml");
	verticalScroll = verticalScrollWidget->rootObject();
	verticalScroll->setWidth(VERTICAL_SCROLL_WIDTH);

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

    const int width = event->size().width();
    const int height = event->size().height();

    header->setWidth(width);
    verticalScroll->setHeight(height - HEADER_HEIGHT - YARD_STICK_HEIGHT - WORKSPACE_HEADER);
	verticalScrollWidget->move(width - VERTICAL_SCROLL_WIDTH, HEADER_HEIGHT + YARD_STICK_HEIGHT + WORKSPACE_HEADER + 1);
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
