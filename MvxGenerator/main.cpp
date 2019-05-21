#include <iostream>
#include <QCoreApplication>
#include <QApplication>
#include <QQuickWidget>
#include <QFileDialog>
#include <QQmlContext>
#include "Handler.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication::setAttribute(Qt::AA_UseOpenGLES, false);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, false);
    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL, true);

    QApplication app(argc, argv);
    PortAudio::init();
    QQuickWidget* view = new QQuickWidget();
    view->rootContext()->setContextProperty("self", new Handler(view));
    view->setSource(QUrl("qrc:/Main.qml"));
    view->show();

    return app.exec();
}