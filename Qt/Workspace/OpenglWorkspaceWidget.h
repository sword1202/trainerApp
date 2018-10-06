#ifndef QOPENGLWORKSPACEWIDGET_H
#define QOPENGLWORKSPACEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLPaintDevice>
#include "WorkspaceDrawer.h"
#include "WorkspaceDrawerWidgetSetup.h"

class OpenGLWorkspaceWidget : public QOpenGLWidget, WorkspaceDrawerWidgetSetup
{
    Q_OBJECT
public:
    explicit OpenGLWorkspaceWidget(QWidget* parent);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // QOPENGLWORKSPACEWIDGET_H
