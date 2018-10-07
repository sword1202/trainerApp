#include "QMetalWidget.h"
#include <QResizeEvent>
#include <QMacNativeWidget>

#define LOCK std::lock_guard<std::mutex> _(sizeMutex)

QMetalWidget::QMetalWidget(QWidget *parent)
    : QWidget(parent)
{
    NSView* view = reinterpret_cast<NSView *>(winId());
    layer = [CAMetalLayer new];
    device = MTLCreateSystemDefaultDevice();
    layer.device = device;
    layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    layer.framebufferOnly = false;
    layer.frame = view.layer.frame;
    [view setLayer:layer];
    [view setWantsLayer:YES];

    sizeChanged = true;
    _size = size();

    CGDirectDisplayID   displayID = CGMainDisplayID();
    CVReturn            error2 = kCVReturnSuccess;
    error2 = CVDisplayLinkCreateWithCGDisplay(displayID, &displayLink);
    if (error2)
    {
        NSLog(@"DisplayLink created with error:%d", error2);
    }

    CVDisplayLinkSetOutputCallback(displayLink, renderCallback, this);
    CVDisplayLinkStart(displayLink);
}

CVReturn QMetalWidget::renderCallback(CVDisplayLinkRef displayLink,
        const CVTimeStamp *inNow,
        const CVTimeStamp *inOutputTime,
        CVOptionFlags flagsIn,
        CVOptionFlags *flagsOut,
        void *displayLinkContext)
{
    static_cast<QMetalWidget*>(displayLinkContext)->renderMetalInternal();
    return kCVReturnSuccess;
}

void QMetalWidget::resizeEvent(QResizeEvent *event) {
    LOCK;
    sizeChanged = true;
    _size = event->size();
}

QMetalWidget::~QMetalWidget()
{
    [layer release];
    [device release];
    CVDisplayLinkStop(displayLink);
}

CAMetalLayer *QMetalWidget::getLayer() const {
    return layer;
}

id <MTLDevice> QMetalWidget::getDevice() const {
    return device;
}

void QMetalWidget::renderMetalInternal() {
    if (!initialized) {
        initMetal();
        initialized = true;
    }
    {
        LOCK;
        if (sizeChanged) {
            metalResize(_size.width(), _size.height());
            sizeChanged = false;
        }
    }
    renderMetal(_size.width(), _size.height());
}

void QMetalWidget::addSubview(NSView* view) {
    NSView* parent = reinterpret_cast<NSView *>(winId());
    [parent addSubview:view positioned:NSWindowAbove relativeTo:nil];
}

void QMetalWidget::addSubWidget(QWidget* widget) {
    QMacNativeWidget *nativeWidget = new QMacNativeWidget();
    nativeWidget->move(0, 0);
    widget->setParent(nativeWidget);
    NSView *nativeWidgetView = reinterpret_cast<NSView *>(nativeWidget->winId());
    addSubview(nativeWidgetView);
    nativeWidget->show();
}
