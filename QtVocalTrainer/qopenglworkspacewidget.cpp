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

    QImage image = QImage("/Users/semyon/Desktop/aaa");
    assert(!image.isNull());
    texture = new QOpenGLTexture(image);
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
    glEnable(GL_TEXTURE_2D);
    texture->bind();
//    glBegin(GL_TRIANGLES);
//    {
//        glTexCoord2f(0, 0);
//        glVertex2d(0, 0);
//        glTexCoord2f(0.5, 0.5);
//        glVertex2d(0.5, 0.5);
//        glTexCoord2f(0, 1.0);
//        glVertex2d(0, 1.0);
//    }
//    glEnd();
    glDisable(GL_TEXTURE_2D);
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
