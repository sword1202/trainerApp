#ifndef WORKSPACEDRAWERWIDGETSETUP_H
#define WORKSPACEDRAWERWIDGETSETUP_H


#include <QWidget>
#include "WorkspaceDrawer.h"

#define USE_NVG_DRAWER
//#define USE_QT_DRAWER

#ifdef USE_QT_DRAWER
#include "QDrawer.h"
typedef QDrawer DrawerImpl;
#else
#include "NvgDrawer.h"
typedef NvgDrawer DrawerImpl;
#endif

class WorkspaceDrawerWidgetSetup
{
    Drawer::Image* createImageFromSvg(const char* url, double width, double height, DrawerImpl *drawer, QWidget *widget);
    void initImages(DrawerImpl *drawer, QWidget *widget);
protected:
    WorkspaceDrawer* workspaceDrawer = nullptr;
    WorkspaceDrawerWidgetSetup();
    void setupWorkspaceDrawer(QWidget* widget, DrawerImpl* drawer);
    void handleResize(QWidget* widget, int w, int h);

public:
    virtual ~WorkspaceDrawerWidgetSetup();

protected:
    void onMouseMove(float x);
    void onMouseClick(float x);
};

#endif // WORKSPACEDRAWERWIDGETSETUP_H