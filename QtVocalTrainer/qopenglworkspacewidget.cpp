#include "qopenglworkspacewidget.h"
#include "NvgDrawer.h"
#include "QDrawer.h"
#include <QMouseEvent>
#include <QOpenGLPaintDevice>
#include <QOpenGLTexture>
#include <QDebug>

QOpenGLWorkspaceWidget::QOpenGLWorkspaceWidget(QWidget* parent) : QOpenGLWidget(parent)
{
#ifdef USE_QT_DRAWER
    QSurfaceFormat format = QSurfaceFormat();
    format.setSamples(8);
    setFormat(format);
#endif
}

QOpenGLTexture* texture;

void QOpenGLWorkspaceWidget::initializeGL() {
    drawer =
#ifdef USE_QT_DRAWER
        new QDrawer(nullptr);
#else
        new NvgDrawer();
#endif
    setupWorkspaceDrawer(this, drawer);
}

void QOpenGLWorkspaceWidget::resizeGL(int w, int h) {
    initDeviceIfNeed();
    handleResize(this, w, h);
#ifdef USE_QT_DRAWER
    int ratio = devicePixelRatio();
    device->setSize(QSize(w * ratio, h * ratio));
    device->setDevicePixelRatio(ratio);
#endif
}

void QOpenGLWorkspaceWidget::initDeviceIfNeed() {
#ifdef USE_QT_DRAWER
    if (!device) {
        device = new QOpenGLPaintDevice();
        drawer->setPaintDevice(device);
    }
#endif
}

void QOpenGLWorkspaceWidget::paintGL() {
#ifdef USE_QT_DRAWER
    initDeviceIfNeed();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glClearColor(1, 1, 1, 1);
#endif
    workspaceDrawer->draw();
    const GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        qDebug() << "GL error: "<<error<<"\n";
    }
}

void QOpenGLWorkspaceWidget::mousePressEvent(QMouseEvent *event) {

}

void QOpenGLWorkspaceWidget::mouseMoveEvent(QMouseEvent *event) {

}

QOpenGLWorkspaceWidget::~QOpenGLWorkspaceWidget() {

}
