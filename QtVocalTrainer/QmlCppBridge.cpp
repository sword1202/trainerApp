#include "QmlCppBridge.h"
#include <iostream>
#include <QScreen>

QmlCppBridge::QmlCppBridge(MainWindow* view) : view(view) {
    connect(view, &MainWindow::resize, view, [=] {
        emit resize();
    });
}

int QmlCppBridge::windowWidth() {
    int i = view->size().width();
    std::cout<<i<<" ";
    return i;
}

int QmlCppBridge::windowHeight() {
    return view->size().height();
}

int QmlCppBridge::dp(qreal dp) {
    return qRound(view->screen()->logicalDotsPerInch() * dp);
}

int QmlCppBridge::dpX(qreal dp) {
    return qRound(view->screen()->logicalDotsPerInchX() * dp);
}

int QmlCppBridge::dpY(qreal dp) {
    qreal i = qRound(view->screen()->logicalDotsPerInchY() * dp);
    std::cout<<i;

    return qRound(view->screen()->logicalDotsPerInchY() * dp);
}
