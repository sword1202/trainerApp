#include "qopenglworkspacewidget.h"
#include "NvgDrawer.h"
#include "MainController.h"
#include "QtUtils/qtutils.h"
#include "fonts.h"
#include <iostream>
#include "Executors.h"

using std::cout;
using std::endl;
using namespace CppUtils;

QOpenGLWorkspaceWidget::QOpenGLWorkspaceWidget(QWidget* parent) : QOpenGLWidget(parent)
{

}

void QOpenGLWorkspaceWidget::initializeGL() {
    NvgDrawer* drawer = new NvgDrawer();

    QByteArray latoRegular = Fonts::latoRegular();
    drawer->registerFont("Lato-Regular", latoRegular.data(), latoRegular.size());
    QByteArray latoBold = Fonts::latoBold();
    drawer->registerFont("Lato-Bold", latoRegular.data(), latoRegular.size());
    workspaceDrawer = new WorkspaceDrawer(drawer, [=] {
        update();
    });
    workspaceDrawer->setTactNumbersFontFamily("Lato-Regular");

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
