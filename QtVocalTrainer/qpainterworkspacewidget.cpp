#include "qpainterworkspacewidget.h"
#include "QDrawer.h"
#include <QResizeEvent>

static QDrawer* drawer;

QPainterWorkspaceWidget::QPainterWorkspaceWidget(QWidget *parent) : QWidget(parent) {
    drawer = new QDrawer(this);
    setupWorkspaceDrawer(this, drawer);
}

void QPainterWorkspaceWidget::paintEvent(QPaintEvent *event) {
    workspaceDrawer->draw();
}

void QPainterWorkspaceWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    handleResize(this, event->size().width(), event->size().height());
}
