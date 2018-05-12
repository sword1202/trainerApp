#include <QGuiApplication>
#include <QQuickView>
#include "MainWindow.h"
#include "QmlCppBridge.h"
#include <QScreen>
#include <QQmlContext>
#include <iostream>
#include "workspacedrawer.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<WorkspaceDrawer>("CppBridge", 1, 0, "WorkspaceDrawer");

    MainWindow viewer;

    QQmlContext* context = viewer.rootContext();
    context->setContextProperty("cpp", new QmlCppBridge(&viewer));
    viewer.setSource(QUrl("qrc:/qml/main.qml"));

    viewer.showMaximized();

    QSize size = viewer.size();
    std::cout<<size.height();
    
    return app.exec();
}
