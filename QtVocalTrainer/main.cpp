#include <QGuiApplication>
#include <QQuickView>
#include "MainWindow.h"
#include "QmlCppBridge.h"
#include <QScreen>
#include <QQmlContext>
#include <iostream>
#include "qmlpitchinputreader.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    MainWindow viewer;

    QQmlContext* context = viewer.rootContext();
    context->setContextProperty("cpp", new QmlCppBridge(&viewer));

    qRegisterMetaType<QmlPitch>("Pitch");
    qmlRegisterType<QmlPitchInputReader>("PitchInputReader", 1, 0, "PitchInputReader");

    viewer.setSource(QUrl("qrc:/qml/main.qml"));

    viewer.showMaximized();
    
    return app.exec();
}
