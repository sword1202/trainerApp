#include <QGuiApplication>
#include <QQuickView>
#include "MainWindow.h"
#include "QmlCppBridge.h"
#include <QScreen>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    MainWindow viewer;

    QScreen* screen = app.screens().at(0);
    QSize screenSize = screen->size();

    QQmlContext* context = viewer.rootContext();
    context->setContextProperty("screenSize", screenSize);
    context->setContextProperty("cpp", new QmlCppBridge(screen));

    viewer.setSource(QUrl("qrc:/main.qml"));
    viewer.showMaximized();

    return app.exec();
}
