#ifndef QOPENGLWORKSPACEWIDGET_H
#define QOPENGLWORKSPACEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLPaintDevice>
#include "WorkspaceDrawer.h"
#include "workspacedrawerwidgetsetup.h"
#include "QDrawer.h"

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

private:
    QDrawer* drawer;
    QOpenGLPaintDevice* device = nullptr;

    void initDeviceIfNeed();
};

#endif // QOPENGLWORKSPACEWIDGET_H
