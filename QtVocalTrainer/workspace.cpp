#include "workspace.h"
#include "TimeUtils.h"
#include <iostream>
#include "QtUtils/qtutils.h"
#include "zoomcontroller.h"
#include "player.h"

using namespace CppUtils;
using namespace std;

Workspace::Workspace(QWidget *parent) : QOpenGLWidget(parent) {
    devicePixelRatio_ = devicePixelRatio();

    // 75 fps
    QtUtils::startRepeatedTimer(this, [=] {
        if (workspaceDrawer.getIntervalsPerSecond() > 0) {
            update();
        }
        return true;
    }, 1000 / 75);

    connect(ZoomController::instance(), &ZoomController::zoomChanged, this, &Workspace::zoomChanged);
    connect(Player::instance(), &Player::isPlayingChanged, this, &Workspace::isPlayingChanged);
}

void Workspace::isPlayingChanged(bool isPlaying) {
    if (isPlaying) {
        workspaceDrawer.setIntervalsPerSecond(Player::instance()->getBeatsPerMinute() / 60.0);
    } else {
        workspaceDrawer.setIntervalsPerSecond(0);
    }
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
