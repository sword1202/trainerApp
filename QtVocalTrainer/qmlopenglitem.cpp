#include <qquickwindow.h>
#include "qmlopenglitem.h"
#include <iostream>

using namespace std;

QmlOpenglItem::QmlOpenglItem(QQuickItem *parent) : QQuickItem(parent) {
    connect(this, &QQuickItem::windowChanged, this, &QmlOpenglItem::handleWindowChanged);
}

void QmlOpenglItem::handleWindowChanged(QQuickWindow *win) {
    if (!win) {
        return;
    }

    win->setClearBeforeRendering(false);

    connect(win, &QQuickWindow::beforeSynchronizing, this, [=] {
            onSync(win);
        }, Qt::DirectConnection);

    connect(win, &QQuickWindow::beforeRendering, this, [=] {
            renderBefore(viewPort);
            win->resetOpenGLState();
        }, Qt::DirectConnection);

    connect(win, &QQuickWindow::afterRendering, this, [=] {
            renderAfter(viewPort);
            win->resetOpenGLState();
        }, Qt::DirectConnection);
}

void QmlOpenglItem::onSync(const QQuickWindow *win) {
    this->viewPort = getViewPort(win);
}

QRect QmlOpenglItem::getViewPort(const QQuickWindow *win) const {
    qreal xpos = x();
    qreal ypos = y();
    qreal deviceRatio = win->devicePixelRatio();

    qreal h = height();
    QPointF viewPortPosition(xpos, (qreal)win->height() - ypos - h);
    QRectF viewPort(viewPortPosition * deviceRatio, size() * deviceRatio);
    return viewPort.toRect();
}

void QmlOpenglItem::renderBefore(const QRect& viewPort) {
    if (!QOpenGLFunctions::isInitialized(QOpenGLFunctions::d_ptr)) {
        initializeOpenGLFunctions();
    }

    //glViewport(viewPort.x(), viewPort.y(), viewPort.width(), viewPort.height());
}

void QmlOpenglItem::renderAfter(const QRect& viewPort) {
    glViewport(viewPort.x(), viewPort.y(), viewPort.width(), viewPort.height());
}
