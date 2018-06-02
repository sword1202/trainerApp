#include "workspace.h"
#include "TimeUtils.h"
#include <iostream>
#include "QtUtils/qtutils.h"
#include "zoomcontroller.h"

using namespace CppUtils;
using namespace std;

Workspace::Workspace(QWidget *parent) : QOpenGLWidget(parent) {
    devicePixelRatio_ = devicePixelRatio();

    // 75 fps
    QtUtils::startRepeatedTimer(this, [=] {
        if (workspaceDrawer.getSpeed() > 0) {
            update();
        }
        return true;
    }, 1000 / 75);

    connect(ZoomController::instance(), SIGNAL(zoomChanged()), this, SLOT(zoomChanged()));
}

void Workspace::zoomChanged() {
    auto* zoom = ZoomController::instance();
    workspaceDrawer.setIntervalWidth((float)zoom->getIntervalWidth());
    workspaceDrawer.setIntervalHeight((float)zoom->getIntervalHeight());
}

void Workspace::initializeGL() {
    QOpenGLWidget::initializeGL();
    zoomChanged();
    workspaceDrawer.setGridColor({0x8B, 0x89, 0xB6, 0x33});
    workspaceDrawer.setAccentGridColor({0x8B, 0x89, 0xB6, 0x80});
    workspaceDrawer.setSpeed(200);
    glDisable(GL_DEPTH_TEST);
}

void Workspace::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);
    workspaceDrawer.resize(w, h, devicePixelRatio_);
}

void Workspace::paintGL() {
    QOpenGLWidget::paintGL();
    workspaceDrawer.draw();
}
