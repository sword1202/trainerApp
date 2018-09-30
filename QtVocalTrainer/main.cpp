﻿#include "mainwindow.h"
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
    VxApp::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication::setAttribute(Qt::AA_UseOpenGLES, false);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, false);
    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL, true);

    QCoreApplication::setOrganizationName("NeboroSoft");
    QCoreApplication::setOrganizationDomain("neborosoft.com");
    QCoreApplication::setApplicationName("Vocal Trainer");

    qRegisterMetaType<QmlPitch>("Pitch");
    qRegisterMetaType<QmlVxPitch>("VxPitch");
    qRegisterMetaType<QmlTimedPitch>("TimedPitch");
    qmlRegisterType<Player>();
    qmlRegisterType<QmlPitchInputReader>();

    VxApp a(argc, argv);
    a.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
