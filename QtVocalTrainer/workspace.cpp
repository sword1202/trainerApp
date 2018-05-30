#include "workspace.h"
#include "TimeUtils.h"
#include <iostream>

using namespace CppUtils;
using namespace std;

Workspace::Workspace(QWidget *parent) : QOpenGLWidget(parent) {
    devicePixelRatio_ = devicePixelRatio();
}


void Workspace::initializeGL() {
    QOpenGLWidget::initializeGL();
    workspaceDrawer.setIntervalWidth(25);
    workspaceDrawer.setIntervalHeight(10);
    workspaceDrawer.setGridColor(nvgRGB(128, 11, 16));
    workspaceDrawer.setAccentGridColor(nvgRGB(255, 11, 16));
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
