#ifndef WORKSPACEVIEW_H
#define WORKSPACEVIEW_H

#include "QMetalWidget.h"
#include "WorkspaceDrawer.h"
#import "../WorkspaceDrawerWidgetSetup.h"

class MetalWorkspaceWidget : public QMetalWidget, public WorkspaceDrawerWidgetSetup
{
public:
    explicit MetalWorkspaceWidget(QWidget *parent = 0);

protected:
    void initMetal() override;
    void renderMetal(int width, int height) override;
    void metalResize(int width, int height) override;
    void onRequestUpdate(QWidget *widget) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Drawer* drawer;
};

#endif // WORKSPACEVIEW_H