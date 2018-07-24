#ifndef QOPENGLWORKSPACEWIDGET_H
#define QOPENGLWORKSPACEWIDGET_H

#include <QOpenGLWidget>
#include "WorkspaceDrawer.h"

class QOpenGLWorkspaceWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    QOpenGLWorkspaceWidget(QWidget* parent);
    ~QOpenGLWorkspaceWidget();

    std::function<void(QMouseEvent*)> onClick;
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    WorkspaceDrawer* workspaceDrawer = nullptr;
};

#endif // QOPENGLWORKSPACEWIDGET_H