#include "mainwindow.h"
#include "app.h"
#include "qmlpitch.h"
#include "qmlvxpitch.h"
#include "qmltimedpitch.h"
#include "player.h"
#include "qmlpitchinputreader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    App::setAttribute(Qt::AA_EnableHighDpiScaling);

    qRegisterMetaType<QmlPitch>("Pitch");
    qRegisterMetaType<QmlVxPitch>("VxPitch");
    qRegisterMetaType<QmlTimedPitch>("TimedPitch");
    qmlRegisterType<Player>();
    qmlRegisterType<QmlPitchInputReader>();

    App a(argc, argv);

    MainWindow w;

    w.showMaximized();

    return a.exec();
}
