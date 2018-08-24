#ifndef QOPENGLWORKSPACEWIDGET_H
#define QOPENGLWORKSPACEWIDGET_H

#include <QOpenGLWidget>
#include "WorkspaceDrawer.h"
#include "workspacedrawerwidgetsetup.h"

class QOpenGLWorkspaceWidget : public QOpenGLWidget, WorkspaceDrawerWidgetSetup
{
    Q_OBJECT
public:
    QOpenGLWorkspaceWidget(QWidget* parent);
    ~QOpenGLWorkspaceWidget();
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // QOPENGLWORKSPACEWIDGET_H