#include "OpenglWorkspaceWidget.h"
#include "NvgDrawer.h"
#include "QDrawer.h"
#include <QMouseEvent>
#include <QOpenGLPaintDevice>
#include <QOpenGLTexture>
#include <QDebug>
#include "MainController.h"
#include <QApplication>
#include <QThread>

OpenGLWorkspaceWidget::OpenGLWorkspaceWidget(QWidget* parent) : QOpenGLWidget(parent)
{
}

QOpenGLTexture* texture;

void OpenGLWorkspaceWidget::initializeGL() {
    Drawer* drawer = new OpenGLNvgDrawer();
    setupWorkspaceDrawer(this, drawer, true);
}

void OpenGLWorkspaceWidget::resizeGL(int w, int h) {
    handleResize(this, w, h);
}

void OpenGLWorkspaceWidget::paintGL() {
    assert(QApplication::instance()->thread() == QThread::currentThread());
    workspaceDrawer->draw();
    const GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        qDebug() << "GL error: "<<error<<"\n";
    }
}

void OpenGLWorkspaceWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        onMouseClick(event->x());
    }
}

void OpenGLWorkspaceWidget::mouseMoveEvent(QMouseEvent *event) {
    onMouseMove(event->x());
}
