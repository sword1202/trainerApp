#include "mainwindow.h"
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

void MainWindow::doMacOsPlatformStaff() {
    NSWindow* window = reinterpret_cast<NSView*>(winId()).window;
    window.backgroundColor = [NSColor colorWithRed:197.0 / 255.0 green:206.0 / 255.0 blue:248.0 / 255.0 alpha:1.0];
    [window setTitlebarAppearsTransparent:YES];
    [window setCollectionBehavior:NSWindowCollectionBehaviorDefault];
}
