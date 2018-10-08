#include "QMetalWidget.h"
#include <QResizeEvent>
#include <QMacNativeWidget>

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

void QMetalWidget::addSubview(NSView* view) {
    NSView* parent = reinterpret_cast<NSView *>(winId());
    [parent addSubview:view positioned:NSWindowAbove relativeTo:nil];
}

QMacNativeWidget*  QMetalWidget::addSubWidget(QWidget* widget) {
    QMacNativeWidget *nativeWidget = new QMacNativeWidget();
    widget->setParent(nativeWidget);
    NSView *nativeWidgetView = reinterpret_cast<NSView *>(nativeWidget->winId());
    addSubview(nativeWidgetView);
    nativeWidget->show();
    return nativeWidget;
}
