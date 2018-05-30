#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QOpenGLWidget>
#include "WorkspaceDrawer.h"

class Workspace : public QOpenGLWidget
{
    Q_OBJECT
public:
    Workspace(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    WorkspaceDrawer workspaceDrawer;
    float devicePixelRatio_;
};

#endif // WORKSPACE_H
