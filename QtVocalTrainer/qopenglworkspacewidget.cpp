#include "qopenglworkspacewidget.h"
#include "NvgDrawer.h"
#include "MainController.h"
#include "QtUtils/qtutils.h"

QOpenGLWorkspaceWidget::QOpenGLWorkspaceWidget(QWidget* parent) : QOpenGLWidget(parent)
{

}

void QOpenGLWorkspaceWidget::initializeGL() {
    workspaceDrawer = new WorkspaceDrawer(new NvgDrawer(), [=] {
        update();
    });
    MainController::instance()->setWorkspaceController(workspaceDrawer);

    QtUtils::startRepeatedTimer(this, [=] {
        if (workspaceDrawer->isRunning()) {
            update();
        }
        return true;
    }, 1000 / 150); // 150fps
}

void QOpenGLWorkspaceWidget::resizeGL(int w, int h) {
    workspaceDrawer->resize(w, h, (float)devicePixelRatioF());
}

void QOpenGLWorkspaceWidget::paintGL() {
    workspaceDrawer->draw();
}

QOpenGLWorkspaceWidget::~QOpenGLWorkspaceWidget() {
    if (workspaceDrawer) {
        delete workspaceDrawer;
    }
}
