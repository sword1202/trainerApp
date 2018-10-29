#include "QMetalWidget.h"
#include <QResizeEvent>
#include <QMacNativeWidget>
#include "../../QtUtils/qtutils.h"

#define LOCK std::lock_guard<std::mutex> _(sizeMutex)

QMetalWidget::QMetalWidget(QWidget *parent)
    : QWidget(parent)
{
    NSView* view = QtUtils::getNSView(this);
    metalView = [[MetalView2 alloc] initWithFrame:view.frame];
    [view addSubview:metalView];
}

void QMetalWidget::resizeEvent(QResizeEvent *event) {
    [metalView resize:event->size().toCGSize()];
}

QMetalWidget::~QMetalWidget() {
}

CAMetalLayer *QMetalWidget::getLayer() const {
    return nil;
    //return metalView.metalLayer;
}

void QMetalWidget::addSubview(NSView* view) {
    NSView* parent = QtUtils::getNSView(this);
    [parent addSubview:view positioned:NSWindowAbove relativeTo:nil];
}

QMacNativeWidget*  QMetalWidget::addSubWidget(QWidget* widget) {
    QMacNativeWidget *nativeWidget = new QMacNativeWidget();
    widget->setParent(nativeWidget);
    NSView *nativeWidgetView = QtUtils::getNSView(nativeWidget);
    addSubview(nativeWidgetView);
    nativeWidget->show();
    return nativeWidget;
}
