#include "mainwindow.h"
#include <QOpenGLWidget>
#include <QHBoxLayout>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    workspace = new Workspace(this);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    workspace->resize(event->size());
}

MainWindow::~MainWindow()
{

}
