#ifndef QMLWORKSPACE_H
#define QMLWORKSPACE_H

#include <QObject>
#include "qmlopenglitem.h"
#include "WorkspaceDrawer.h"
#include <QOpenGLShaderProgram>

class QmlWorkspace : public QmlOpenglItem
{
public:
    QmlWorkspace();

protected:
    virtual void renderBefore(const QRect &viewPort, qreal devicePixelRatio) override;
private:
    WorkspaceDrawer workspaceDrawer;
};

#endif // QMLWORKSPACE_H