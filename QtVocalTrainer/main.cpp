#include <QGuiApplication>
#include "QmlCppBridge.h"
#include <QQmlContext>
#include <iostream>
#include <QQmlApplicationEngine>
#include "qmlpitchinputreader.h"
#include "app.h"
#include "qmlplayer.h"
#include "qmlvxpitch.h"

int main(int argc, char *argv[])
{
    App::setAttribute(Qt::AA_EnableHighDpiScaling);

    qRegisterMetaType<QmlPitch>("Pitch");
    qRegisterMetaType<QmlVxPitch>("VxPitch");
    qRegisterMetaType<QmlTimedPitch>("TimedPitch");
    qRegisterMetaType<QmlVxPitchArray>("VxPitchArray");
    qmlRegisterType<QmlPitchInputReader>("PitchInputReader", 1, 0, "PitchInputReader");
    qmlRegisterType<QmlPlayer>("Player", 1, 0, "Player");

    App app(argc, argv);

    return app.exec();
}
