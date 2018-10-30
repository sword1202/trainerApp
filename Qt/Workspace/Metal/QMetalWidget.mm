#include "QMetalWidget.h"
#include <QResizeEvent>
#include <QMacNativeWidget>
#include "../../QtUtils/qtutils.h"

#define LOCK std::lock_guard<std::mutex> _(sizeMutex)

QMetalWidget::QMetalWidget(QWidget *parent)
    : QWidget(parent)
{
    NSView* view = QtUtils::getNSView(this);
    metalView = [[MetalView alloc] initWithFrame:view.frame
                 callback:this andDevicePixelRatio:devicePixelRatioF()];
    [view addSubview:metalView];
}

void QMetalWidget::resizeEvent(QResizeEvent *event) {
    [metalView resize:event->size().toCGSize()];
}

CAMetalLayer *QMetalWidget::getLayer() const {
    return metalView.metalLayer;
}

void QMetalWidget::addSubview(NSView* view) {
    NSView* parent = QtUtils::getNSView(this);
    [parent addSubview:view positioned:NSWindowAbove relativeTo:nil];
}

QMacNativeWidget*  QMetalWidget::addSubWidget(QWidget* widget) {
    auto *nativeWidget = new QMacNativeWidget();
    widget->setParent(nativeWidget);
    NSView *nativeWidgetView = QtUtils::getNSView(nativeWidget);
    addSubview(nativeWidgetView);
    nativeWidget->show();
    return nativeWidget;
}
