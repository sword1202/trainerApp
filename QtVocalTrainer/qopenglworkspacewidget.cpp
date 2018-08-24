#include "qopenglworkspacewidget.h"
#include "NvgDrawer.h"
#include "QDrawer.h"
#include <QMouseEvent>
#include <QOpenGLPaintDevice>

QOpenGLWorkspaceWidget::QOpenGLWorkspaceWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    QSurfaceFormat format = QSurfaceFormat();
    format.setSamples(8);
    setFormat(format);
}

void QOpenGLWorkspaceWidget::initializeGL() {
    drawer = new QDrawer(nullptr);
    setupWorkspaceDrawer(this, drawer);
}

void QOpenGLWorkspaceWidget::resizeGL(int w, int h) {
    initDeviceIfNeed();
    handleResize(this, w, h);
    int ratio = devicePixelRatio();
    device->setSize(QSize(w * ratio, h * ratio));
    device->setDevicePixelRatio(ratio);
}

void QOpenGLWorkspaceWidget::initDeviceIfNeed() {
    if (!device) {
        device = new QOpenGLPaintDevice();
        drawer->setPaintDevice(device);
    }
}

void QOpenGLWorkspaceWidget::paintGL() {
    initDeviceIfNeed();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glClearColor(1, 1, 1, 1);
    workspaceDrawer->draw();
}

void QOpenGLWorkspaceWidget::mousePressEvent(QMouseEvent *event) {

}

void QOpenGLWorkspaceWidget::mouseMoveEvent(QMouseEvent *event) {

}

QOpenGLWorkspaceWidget::~QOpenGLWorkspaceWidget() {

}
