#include "qopenglworkspacewidget.h"
#include "NvgDrawer.h"
#include "MainController.h"
#include "QtUtils/qtutils.h"
#include "fonts.h"
#include <iostream>

using std::cout;
using std::endl;

QOpenGLWorkspaceWidget::QOpenGLWorkspaceWidget(QWidget* parent) : QOpenGLWidget(parent)
{

}

void QOpenGLWorkspaceWidget::initializeGL() {
    NvgDrawer* drawer = new NvgDrawer();

    QByteArray latoRegularFontData = Fonts::latoRegular();
    drawer->registerFont("Lato-Regular", latoRegularFontData.data(), latoRegularFontData.size());
    workspaceDrawer = new WorkspaceDrawer(drawer, [=] {
        update();
    });
    MainController::instance()->setWorkspaceController(workspaceDrawer);

    QtUtils::startRepeatedTimer(this, [=] {
        if (workspaceDrawer->isRunning()) {
            update();
        }
        return true;
    }, 1000 / 150); // 150fps

    setMouseTracking(true);
}

void QOpenGLWorkspaceWidget::resizeGL(int w, int h) {
    workspaceDrawer->resize(w, h, (float)devicePixelRatioF());
}

void QOpenGLWorkspaceWidget::paintGL() {
    workspaceDrawer->draw();
}

void QOpenGLWorkspaceWidget::mousePressEvent(QMouseEvent *event) {
    if (onClick) {
        onClick(event);
    }
    QWidget::mousePressEvent(event);
}

QOpenGLWorkspaceWidget::~QOpenGLWorkspaceWidget() {
    if (workspaceDrawer) {
        delete workspaceDrawer;
    }
}
