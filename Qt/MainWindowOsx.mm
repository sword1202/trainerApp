#include "MainWindow.h"
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#include "BaseMainWindow.h"
#ifdef USE_METAL
#include "MacOS/workspaceview.h"
#endif

void BaseMainWindow::doMacOsPlatformStaff(QColor windowBorderColor) {
    NSView* view = reinterpret_cast<NSView*>(winId());
    NSWindow* window = view.window;
    window.backgroundColor = [NSColor colorWithRed:windowBorderColor.redF()
            green:windowBorderColor.greenF() blue:windowBorderColor.blueF() alpha:windowBorderColor.alphaF()];
    [window setTitlebarAppearsTransparent:YES];
    [window setCollectionBehavior:NSWindowCollectionBehaviorDefault];
}

#ifdef USE_METAL

void MainWindow::setupWorkspaceView() {
    workspaceView = new WorkspaceView(this);
}

#endif
