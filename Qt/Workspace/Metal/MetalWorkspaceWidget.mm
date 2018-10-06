#import "MainController.h"
#include "MetalWorkspaceWidget.h"
#import "NvgDrawer.h"
#include "Executors.h"
#include <iostream>

using namespace std;
using namespace CppUtils;

MetalWorkspaceWidget::MetalWorkspaceWidget(QWidget *parent) : QMetalWidget(parent) {

}

void MetalWorkspaceWidget::renderMetal(int width, int height) {
    if (workspaceDrawer) {
        workspaceDrawer->draw();
    }
}

void MetalWorkspaceWidget::metalResize(int width, int height) {
    handleResize(this, width, height);
}

void MetalWorkspaceWidget::initMetal() {
    Drawer* drawer = new MetalNvgDrawer((__bridge void *) getLayer());
    Executors::ExecuteOnMainThread([=] {
        setupWorkspaceDrawer(this, drawer, false);
    });
}
