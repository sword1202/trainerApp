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
    //workspaceDrawer->draw();
    drawer->clear();
    drawer->beginFrame(width, height, devicePixelRatio());
    drawer->setFillColor(DrawerColor::red());
    drawer->fillRect(0, 0, 200, 200);
    drawer->endFrame();
}

void MetalWorkspaceWidget::metalResize(int width, int height) {
    //handleResize(this, width, height);
}

void MetalWorkspaceWidget::initMetal() {
    drawer = new MetalNvgDrawer((__bridge void *) getLayer());
    //setupWorkspaceDrawer(this, drawer, false);
}

void MetalWorkspaceWidget::onRequestUpdate(QWidget *widget) {
    // do nothing, metal is updated automatically
}
