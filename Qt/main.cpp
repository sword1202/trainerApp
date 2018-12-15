#include <QApplication>
#include <QFontDatabase>

#include "VxApp.h"
#include "QmlPitch.h"
#include "QtMvxPlayer.h"
#include "Welcome/WelcomeWindow.h"

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
    qmlRegisterType<QtMvxPlayer>();

    VxApp a(argc, argv);
    a.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    WelcomeWindow w;
    w.show();

    return a.exec();
}
