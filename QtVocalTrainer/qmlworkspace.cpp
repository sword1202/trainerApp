#include "qmlworkspace.h"
#include "WorkspaceDrawer.h"

QmlWorkspace::QmlWorkspace()
{

}

void QmlWorkspace::renderBefore(const QRect &viewPort, qreal devicePixelRatio) {
    QmlOpenglItem::renderBefore(viewPort, devicePixelRatio);

    glDisable(GL_DEPTH_TEST);

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    workspaceDrawer.draw(viewPort.width(), viewPort.height(), (float)devicePixelRatio);
}
