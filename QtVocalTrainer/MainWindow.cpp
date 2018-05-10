#include "MainWindow.h"

#ifndef __APPLE__
MainWindow::MainWindow() {}
#endif

void MainWindow::resizeEvent(QResizeEvent *event) {
    QQuickView::resizeEvent(event);
    emit resize();
}
