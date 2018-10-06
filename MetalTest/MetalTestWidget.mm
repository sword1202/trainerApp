//
// Created by Semyon Tikhonenko on 10/6/18.
//

#include "MetalTestWidget.h"

void MetalTestWidget::initMetal() {
    drawer = new MetalNvgDrawer((__bridge void*)getLayer());
}

void MetalTestWidget::renderMetal(int width, int height) {
    drawer->clear();
    drawer->beginFrame(width, height, devicePixelRatio());
    drawer->setFillColor(DrawerColor::black());
    drawer->fillRect(0, 0, 200, 200);
    drawer->endFrame();
}

void MetalTestWidget::metalResize(int width, int height) {

}
