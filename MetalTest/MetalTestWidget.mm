//
// Created by Semyon Tikhonenko on 10/6/18.
//

#import <QMacNativeWidget>
#include "MetalTestWidget.h"
#include <QLabel>

void MetalTestWidget::initMetal() {
    drawer = new MetalNvgDrawer((__bridge void*)getLayer());
}

void MetalTestWidget::renderMetal(int width, int height) {
    drawer->clear();
    drawer->beginFrame(width, height, devicePixelRatio());
    drawer->setFillColor(DrawerColor::red());
    drawer->fillRect(0, 0, 200, 200);
    drawer->endFrame();
}

void MetalTestWidget::metalResize(int width, int height) {

}

MetalTestWidget::MetalTestWidget() {
    QLabel* label = new QLabel("Vasia");
    addSubWidget(label);
}
