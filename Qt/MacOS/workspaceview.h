#ifndef WORKSPACEVIEW_H
#define WORKSPACEVIEW_H

#include "qmetalwidget.h"
#include "WorkspaceDrawer.h"

class WorkspaceView : public QMetalWidget
{
public:
    WorkspaceView(QWidget *parent = 0);

protected:
    void renderMetal(int width, int height) override;
    void metalResize(int width, int height) override;

private:
    std::unique_ptr<WorkspaceDrawer> workspaceDrawer;
    void initDrawer(int width, int height);
};

#endif // WORKSPACEVIEW_H