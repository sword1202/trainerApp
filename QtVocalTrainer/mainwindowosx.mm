#include "mainwindow.h"
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#ifdef USE_METAL
#include "MacOS/workspaceview.h"
#endif

void MainWindow::doMacOsPlatformStaff() {
    NSView* view = reinterpret_cast<NSView*>(winId());
    NSWindow* window = view.window;
    window.backgroundColor = [NSColor colorWithRed:197.0 / 255.0 green:206.0 / 255.0 blue:248.0 / 255.0 alpha:1.0];
    [window setTitlebarAppearsTransparent:YES];
    [window setCollectionBehavior:NSWindowCollectionBehaviorDefault];
}

#ifdef USE_METAL

void MainWindow::setupWorkspaceView() {
    workspaceView = new WorkspaceView(this);
}

#endif
