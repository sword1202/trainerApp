#include "MainWindow.h"

void MainWindow::resizeEvent(QResizeEvent *event) {
    QQuickView::resizeEvent(event);
    emit resize();
}
