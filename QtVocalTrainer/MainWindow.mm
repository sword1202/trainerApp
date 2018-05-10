#include "MainWindow.h"
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

MainWindow::MainWindow() {
    NSWindow* window = reinterpret_cast<NSView*>(winId()).window;
    window.backgroundColor = [NSColor blueColor];
    [window setTitlebarAppearsTransparent:YES];
    [window setCollectionBehavior:NSWindowCollectionBehaviorDefault];
    QObject::connect(this, &QQuickView::windowStateChanged, this, [=]() {
        [window setCollectionBehavior:NSWindowCollectionBehaviorDefault];
    });
}
