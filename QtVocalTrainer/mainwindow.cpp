#include "mainwindow.h"
#include <QOpenGLWidget>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QQuickItem>

constexpr int HEADER_HEIGHT = 75;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    workspace = new Workspace(this);
    QQuickWidget* headerWidget = new QQuickWidget(this);
    headerWidget->setSource(QUrl("qrc:/qml/Header.qml"));
    header = headerWidget->rootObject();
    workspace->move(0, HEADER_HEIGHT);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    workspace->resize(event->size());
    int headerWidth = event->size().width();
    //header->resize(headerWidth, 75);
    header->setWidth(headerWidth);
    header->setHeight(HEADER_HEIGHT);
}

MainWindow::~MainWindow()
{

}
