#include "mainwindow.h"
#include "app.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    App a(argc, argv);
    MainWindow w;
    w.showMaximized();

    return a.exec();
}
