#include "qpainterworkspacewidget.h"
#include "QDrawer.h"
#include <QResizeEvent>

static QDrawer* drawer;

QPainterWorkspaceWidget::QPainterWorkspaceWidget(QWidget *parent) : QWidget(parent) {
    drawer = new QDrawer(this);
    setupWorkspaceDrawer(this, drawer);
}

void QPainterWorkspaceWidget::paintEvent(QPaintEvent *event) {
    drawer->beginFrame(width(), height(), devicePixelRatio());
    drawer->setFillColor(DrawerColor::white());
    drawer->fillRect(0, 0, 100, 100);
    drawer->endFrame();
//    workspaceDrawer->draw();
}

void QPainterWorkspaceWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    handleResize(this, event->size().width(), event->size().height());
}
