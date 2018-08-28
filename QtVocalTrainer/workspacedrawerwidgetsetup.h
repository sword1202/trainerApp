#ifndef WORKSPACEDRAWERWIDGETSETUP_H
#define WORKSPACEDRAWERWIDGETSETUP_H


#include <QWidget>
#include "WorkspaceDrawer.h"

//#define USE_NVG_DRAWER
#define USE_QT_DRAWER

#ifdef USE_QT_DRAWER
#include "QDrawer.h"
typedef QDrawer DrawerImpl;
#else
#include "NvgDrawer.h"
typedef NvgDrawer DrawerImpl;
#endif

class WorkspaceDrawerWidgetSetup
{
    void initPlayHeadTriangleImage(DrawerImpl *drawer, QWidget *widget);
protected:
    WorkspaceDrawer* workspaceDrawer = nullptr;
    WorkspaceDrawerWidgetSetup();
    void setupWorkspaceDrawer(QWidget* widget, DrawerImpl* drawer);
    void handleResize(QWidget* widget, int w, int h);

public:
    virtual ~WorkspaceDrawerWidgetSetup();
};

#endif // WORKSPACEDRAWERWIDGETSETUP_H