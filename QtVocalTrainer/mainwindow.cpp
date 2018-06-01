#include "mainwindow.h"
#include "QmlCppBridge.h"
#include <QOpenGLWidget>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QQuickItem>
#include <QQmlContext>

constexpr int HEADER_HEIGHT = 75;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    workspace = new Workspace(this);
    QQuickWidget* headerWidget = new QQuickWidget(this);
    headerWidget->rootContext()->setContextProperty("cpp", new QmlCppBridge(this));
    headerWidget->setSource(QUrl("qrc:/qml/Header.qml"));
    header = headerWidget->rootObject();
    header->setFlag(QQuickItem::ItemHasContents, true);
    workspace->move(0, HEADER_HEIGHT);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    workspace->resize(event->size());
    int headerWidth = event->size().width();
    header->setWidth(headerWidth);
    header->setHeight(HEADER_HEIGHT);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    header->update();
}

void MainWindow::updateHeader() {
    if (header) {
        header->polish();
    }
}

MainWindow::~MainWindow()
{

}
