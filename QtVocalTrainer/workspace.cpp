#include "workspace.h"
#include "TimeUtils.h"
#include <iostream>
#include "QtUtils/qtutils.h"
#include "zoomcontroller.h"
#include "player.h"
#include "qmlpitchinputreader.h"

using namespace CppUtils;
using namespace std;

static const int PITCH_RADIUS = 3;

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
    connect(Player::instance(), &Player::sourceChanged, this, &Workspace::playerSourceChanged);
    connect(Player::instance(), &Player::seekChanged, this, &Workspace::seekChanged);

    connect(ZoomController::instance(), &ZoomController::firstPitchChanged, this, &Workspace::firstPitchChanged);

    QmlPitchInputReader* pitchInputReader = QmlPitchInputReader::instance();
    workspaceDrawer.setPitchesCollector(pitchInputReader);
}

void Workspace::firstPitchChanged() {
    workspaceDrawer.setFirstPitchPerfectFrequencyIndex(
            ZoomController::instance()->getFirstPitchPerfectFrequencyIndex());
}

void Workspace::isPlayingChanged(bool isPlaying) {
    if (isPlaying) {
        workspaceDrawer.setIntervalsPerSecond(Player::instance()->getBeatsPerMinute() / 60.0);
    } else {
        workspaceDrawer.setIntervalsPerSecond(0);
    }
}

void Workspace::playerSourceChanged() {
    VxFile *vxFile = new VxFile(Player::instance()->getVxFile());
    renderingQueue.post([=] {
        workspaceDrawer.setVxFile(vxFile);
    });
}

void Workspace::zoomChanged() {
    auto* zoom = ZoomController::instance();
    workspaceDrawer.setIntervalWidth((float)zoom->getIntervalWidth());
    workspaceDrawer.setIntervalHeight((float)zoom->getIntervalHeight());
}

void Workspace::seekChanged(double seek) {
    workspaceDrawer.setHorizontalOffset(seek * workspaceDrawer.getIntervalWidth() *
            workspaceDrawer.getIntervalsPerSecond());
}

void Workspace::initializeGL() {
    QOpenGLWidget::initializeGL();
    zoomChanged();
    firstPitchChanged();
    workspaceDrawer.setGridColor({0x8B, 0x89, 0xB6, 0x33});
    workspaceDrawer.setAccentGridColor({0x8B, 0x89, 0xB6, 0x80});
    workspaceDrawer.setPitchGraphColor({0xFF, 0x5E, 0x85, 0xFF});
    workspaceDrawer.setPitchColor({0x6E, 0x7E, 0xC5, 0xFF});
    workspaceDrawer.setPitchRadius(PITCH_RADIUS);
    //workspaceDrawer.setIntervalsPerSecond(3);
    glDisable(GL_DEPTH_TEST);
}

void Workspace::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);
    workspaceDrawer.resize(w, h, devicePixelRatio_);
}

void Workspace::paintGL() {
    QOpenGLWidget::paintGL();
    renderingQueue.process();
    workspaceDrawer.draw();
}
