#include "QMetalWidget.h"
#include <QResizeEvent>

#define LOCK std::lock_guard<std::mutex> _(sizeMutex)

QMetalWidget::QMetalWidget(QWidget *parent)
    : QWidget(parent)
{
    NSView* view = reinterpret_cast<NSView *>(winId());
    metalView = [[MetalView alloc] initWithFrame:view.frame callback:this];
    [view addSubview:metalView];
}

void QMetalWidget::resizeEvent(QResizeEvent *event) {
    [metalView resize:event->size().toCGSize()];
}

QMetalWidget::~QMetalWidget() {
    [metalView release];
}

CAMetalLayer *QMetalWidget::getLayer() const {
    return metalView.metalLayer;
}
