#include <qquickwindow.h>
#include "qmlopenglitem.h"
#include <iostream>
#include <QTimer>

using namespace std;

QmlOpenglItem::QmlOpenglItem(QQuickItem *parent) : QQuickItem(parent) {
    connect(this, &QQuickItem::windowChanged, this, &QmlOpenglItem::handleWindowChanged);
}

void QmlOpenglItem::handleWindowChanged(QQuickWindow *win) {
    if (!win) {
        return;
    }

    this->devicePixelRation = win->devicePixelRatio();
    win->setClearBeforeRendering(false);

    connect(win, &QQuickWindow::beforeSynchronizing, this, [=] {
            onSync(win);
        }, Qt::DirectConnection);

    connect(win, &QQuickWindow::beforeRendering, this, [=] {
            renderBefore(viewPort, devicePixelRation);
            //win->resetOpenGLState();
        }, Qt::DirectConnection);

    connect(win, &QQuickWindow::afterRendering, this, [=] {
            //renderAfter(viewPort, devicePixelRation);
            //win->resetOpenGLState();
        }, Qt::DirectConnection);

    if (timer) {
        delete timer;
    }

    timer = new QTimer();
    // 60 fps
    timer->setInterval(1000 / 60);
    connect(timer, &QTimer::timeout, [=] {
        //update();
    });
    timer->start();
}

void QmlOpenglItem::onSync(const QQuickWindow *win) {
    this->viewPort = getViewPort(win);
    win->devicePixelRatio();
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

void QmlOpenglItem::renderBefore(const QRect& viewPort, qreal devicePixelRatio) {
    glViewport(viewPort.x(), viewPort.y(), viewPort.width(), viewPort.height());
}

void QmlOpenglItem::renderAfter(const QRect& viewPort, qreal devicePixelRatio) {
    glViewport(viewPort.x(), viewPort.y(), viewPort.width(), viewPort.height());
}
