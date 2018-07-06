#import "MainController.h"
#include "workspaceview.h"
#import "NvgDrawer.h"
#include <iostream>

using namespace std;

WorkspaceView::WorkspaceView(QWidget *parent) : QMetalWidget(parent) {

}

void WorkspaceView::renderMetal(int width, int height) {
    if (!workspaceDrawer) {
        initDrawer(width, height);
    }

    workspaceDrawer->draw();
}

void WorkspaceView::initDrawer(int width, int height) {
    Drawer* drawer = new NvgDrawer((__bridge void *) getLayer());
    workspaceDrawer = std::make_unique<WorkspaceDrawer>(drawer);
    VxApp::instance()->setWorkspaceController(workspaceDrawer.get());

    cout<<"width = "<<width<<" height = "<<height<<"\n";
    workspaceDrawer->resize(width, height, devicePixelRatio());
}

void WorkspaceView::metalResize(int width, int height) {
    initDrawer(width, height);
}
