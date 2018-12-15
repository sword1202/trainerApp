#include "MainWindow.h"
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#include "BaseMainWindow.h"
#include "Utils/QtUtils.h"

void BaseMainWindow::doMacOsPlatformStaff(QColor windowBorderColor) {
    NSView* view = QtUtils::getNSView(this);
    NSWindow* window = view.window;
    window.backgroundColor = [NSColor colorWithRed:windowBorderColor.redF()
            green:windowBorderColor.greenF() blue:windowBorderColor.blueF() alpha:windowBorderColor.alphaF()];
    [window setTitlebarAppearsTransparent:YES];
    [window setCollectionBehavior:NSWindowCollectionBehaviorDefault];
}
