#include <QApplication>
#import "MetalTestWidget.h"
#include "WorkspaceView.h"
#include <QMainWindow>
#import <AppKit/AppKit.h>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication::setAttribute(Qt::AA_UseOpenGLES, false);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, false);
    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL, true);

    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    QMainWindow mainWindow;
//    MetalTestWidget* w = new MetalTestWidget(&mainWindow);
//    w->setFixedSize(500, 500);
//    w->move(0, 0);
//    w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mainWindow.setFixedSize(500, 500);

    NSView* parent = reinterpret_cast<NSView *>(mainWindow.winId());
//    NSTextView *text = [[NSTextView alloc] initWithFrame : NSMakeRect(0, 0, 300, 300)];
//    [text setString:@"the string"];

    WorkspaceView* workspaceView = [[WorkspaceView alloc] initWithFrame:NSMakeRect(0, 0, 300, 300)];
    [parent addSubview:workspaceView positioned:NSWindowAbove relativeTo:nil];

    mainWindow.show();

    [workspaceView release];
    return a.exec();
}
