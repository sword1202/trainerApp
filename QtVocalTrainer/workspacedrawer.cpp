#include "workspacedrawer.h"
#include <QtGui/QOpenGLContext>

WorkspaceDrawer::WorkspaceDrawer(QQuickItem *parent) : QmlOpenglItem(parent) {

}

void WorkspaceDrawer::renderBefore(const QRect &viewPort) {
//    glViewport(viewPort.x(), viewPort.y(), viewPort.width(), viewPort.height());
//    glViewport(0, 0, 100, 100);
//
//    glClearColor(0.1, 0.5, 0.5, 1);
//    glClear(GL_COLOR_BUFFER_BIT);
}

void WorkspaceDrawer::renderAfter(const QRect &viewPort) {
    glViewport(viewPort.x(), viewPort.y(), viewPort.width(), viewPort.height());

    glClearColor(0.1, 0.5, 0.5, 1);
    glScissor(viewPort.x(), viewPort.y(), viewPort.width(), viewPort.height());
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}
