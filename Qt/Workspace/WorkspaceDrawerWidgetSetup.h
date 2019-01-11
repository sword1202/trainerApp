#ifndef WORKSPACEDRAWERWIDGETSETUP_H
#define WORKSPACEDRAWERWIDGETSETUP_H


#include <QWidget>
#include "WorkspaceDrawer.h"

class WorkspaceDrawerWidgetSetup
{
    Drawer::Image* createImageFromSvg(const char* url, double width, double height, Drawer *drawer, QWidget *widget);
    void initImages(Drawer *drawer, QWidget *widget);
protected:
    WorkspaceDrawer* workspaceDrawer = nullptr;
    WorkspaceDrawerWidgetSetup();
    void setupWorkspaceDrawer(QWidget* widget, Drawer* drawer, bool useUpdateLoop);
    void handleResize(QWidget* widget, int w, int h);

public:
    virtual ~WorkspaceDrawerWidgetSetup();
protected:
    virtual void onMouseMove(float x);
    virtual void onMouseClick(float x);
    virtual void onRequestUpdate(QWidget* widget);
};

#endif // WORKSPACEDRAWERWIDGETSETUP_H