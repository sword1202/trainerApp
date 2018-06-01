#include "workspace.h"
#include "TimeUtils.h"
#include <iostream>
#include "qtutils.h"

using namespace CppUtils;
using namespace std;

Workspace::Workspace(QWidget *parent) : QOpenGLWidget(parent) {
    devicePixelRatio_ = devicePixelRatio();

    QtUtils::startRepeatedTimer(this, [=] {
            update();
            return true;
        }, 1000 / 60); // 60fps
}


void Workspace::initializeGL() {
    QOpenGLWidget::initializeGL();
    workspaceDrawer.setIntervalWidth(25);
    workspaceDrawer.setIntervalHeight(10);
    workspaceDrawer.setGridColor(nvgRGBA(0x8B, 0x89, 0xB6, 0x33));
    workspaceDrawer.setAccentGridColor(nvgRGBA(0x8B, 0x89, 0xB6, 0x80));
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
    update();
}
