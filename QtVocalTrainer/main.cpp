#include "mainwindow.h"
#include "app.h"
#include "qmlpitch.h"
#include "qmlvxpitch.h"
#include "qmltimedpitch.h"
#include "player.h"
#include "MacOS/workspaceview.h"
#include "qmlpitchinputreader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QtVxApp::setAttribute(Qt::AA_EnableHighDpiScaling);

    qRegisterMetaType<QmlPitch>("Pitch");
    qRegisterMetaType<QmlVxPitch>("VxPitch");
    qRegisterMetaType<QmlTimedPitch>("TimedPitch");
    qmlRegisterType<Player>();
    qmlRegisterType<QmlPitchInputReader>();

    QtVxApp a(argc, argv);
    a.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
