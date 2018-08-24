#ifndef WORKSPACEDRAWERWIDGETSETUP_H
#define WORKSPACEDRAWERWIDGETSETUP_H


#include <QWidget>
#include "WorkspaceDrawer.h"

class WorkspaceDrawerWidgetSetup
{
    void initPlayHeadImage(Drawer *drawer, QWidget* widget);
protected:
    WorkspaceDrawer* workspaceDrawer = nullptr;
    WorkspaceDrawerWidgetSetup();
    void setupWorkspaceDrawer(QWidget* widget);
    void handleResize(QWidget* widget, int w, int h);

public:
    virtual ~WorkspaceDrawerWidgetSetup();
};

#endif // WORKSPACEDRAWERWIDGETSETUP_H