#include <QGuiApplication>
#include <QQuickView>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    MainWindow viewer;

    viewer.setSource(QUrl("qrc:/main.qml"));
    viewer.showMaximized();

    return app.exec();
}
